#include "Core/LineModifier/NumelementModifier.h"

#include "Core/CompileState.h"

#include "Utilities/Algorithm.h"

size_t CountMatches(const std::string& text, const std::string& match)
{
    size_t count = 0;
    size_t pos   = text.find(match);

    while (pos != NOT_FOUND)
    {
        count++;
        pos = text.find(match, pos + match.length());
    }

    return count;
}

bool TrackLayer(int& layer,
                const std::string& text,
                const std::string& open,
                const std::string& close,
                std::function<bool()> continue_callback)
{
    size_t pos  = text.find(open);
    size_t pos2 = text.find(close);

    while (pos != NOT_FOUND || pos2 != NOT_FOUND)
    {
        if (pos < pos2)
        {
            layer++;
            pos = text.find(open, pos + open.length());
        }
        else if (pos2 < pos)
        {
            layer--;
            pos2 = text.find(close, pos2 + close.length());

            if (layer != 0) continue;

            if (continue_callback()) continue;

            return false;
        }
    }

    return true;
}

nemesis::NumelementModifier::NumelementModifier(
    size_t begin, size_t end, const std::string& name, size_t linenum, const std::filesystem::path& filepath)
    : nemesis::LineModifier(begin, end)
    , Name(name)
    , LineNum(linenum)
    , FilePath(filepath)
{
}

void nemesis::NumelementModifier::Apply(VecStr& blocks, nemesis::CompileState& state) const
{
    auto element_counter = std::make_shared<size_t>(0);
    auto param_layer     = std::make_shared<int>(0);
    auto object_layer    = std::make_shared<int>(0);

    auto handler_add       = std::make_shared<void*>(nullptr);
    auto error_handler_add = std::make_shared<std::function<void()>*>(nullptr);

    auto start_ptr = std::make_shared<nemesis::Line*>(nullptr);
    auto guid_ptr  = std::make_shared<std::string>("{" + nemesis::generate_guid() + "}");

    bool is_vector = Name == "quadVariableValues";

    ClearCoveredBlocks(blocks);
    blocks[Begin] = *guid_ptr;

    auto counting_handler = std::make_shared<std::function<void(nemesis::Line&)>>([](nemesis::Line&) {});

    std::function<void(nemesis::Line&)> count_check = [is_vector,
                                                       element_counter,
                                                       param_layer,
                                                       object_layer,
                                                       handler_add,
                                                       error_handler_add,
                                                       start_ptr,
                                                       guid_ptr,
                                                       counting_handler,
                                                       &state](nemesis::Line& line)
    {
        if (*object_layer <= 0)
        {
            auto cont = TrackLayer(
                *param_layer,
                line,
                "<hkparam ",
                "</hkparam>",
                [is_vector, start_ptr, guid_ptr, element_counter, handler_add, error_handler_add, &state]()
                {
                    size_t element = *element_counter / (is_vector ? 4 : 1);
                    (*start_ptr)->replace(*guid_ptr, std::to_string(element));

                    state.RemoveEOFHandler(*error_handler_add);
                    state.RemoveAddLineHandler(*handler_add);
                    return false;
                });

            if (!cont) return;
        }

        (*counting_handler)(line);
    };

    auto first_check = std::make_shared<std::function<void(nemesis::Line&)>>(
        [is_vector,
         element_counter,
         param_layer,
         object_layer,
         handler_add,
         error_handler_add,
         start_ptr,
         guid_ptr,
         counting_handler,
         count_check,
         &state](nemesis::Line& line)
        {
            if (line.empty()) return;

            if (line.find("<hkobject>") != NOT_FOUND)
            {
                *counting_handler = [element_counter, object_layer](nemesis::Line& line)
                {
                    TrackLayer(*object_layer,
                               line,
                               "<hkobject>",
                               "</hkobject>",
                               [element_counter]()
                               {
                                   (*element_counter)++;
                                   return true;
                               });
                };
            }
            else if (line.find("<hkcstring>") != NOT_FOUND)
            {
                *counting_handler = [element_counter](nemesis::Line& line)
                { *element_counter += CountMatches(line, "<hkcstring>"); };
            }
            else if (line.find("(") != NOT_FOUND)
            {
                *counting_handler = [element_counter, object_layer](nemesis::Line& line)
                {
                    int& layer_count = *object_layer;

                    for (auto& ch : line)
                    {
                        switch (ch)
                        {
                            case '(':
                            {
                                layer_count++;
                                break;
                            }
                            case ')':
                            {
                                if (--layer_count != 0) break;

                                (*element_counter)++;
                                break;
                            }
                        }
                    }
                };
            }
            else if (line.find("<") == NOT_FOUND)
            {
                *counting_handler = [element_counter](nemesis::Line& line)
                {
                    std::string e;
                    std::istringstream iss(line.ToString());

                    while (iss >> e)
                    {
                        (*element_counter)++;
                    }
                };
            }

            (*counting_handler)(line);

            if (*object_layer == 0)
            {
                int layer_count = *param_layer;
                TrackLayer(layer_count,
                           line,
                           "<hkparam ",
                           "</hkparam>",
                           [is_vector,
                            start_ptr,
                            guid_ptr,
                            element_counter,
                            handler_add,
                            error_handler_add,
                            &state]()
                           {
                               size_t element = *element_counter / (is_vector ? 4 : 1);
                               (*start_ptr)->replace(*guid_ptr, std::to_string(element));

                               state.RemoveEOFHandler(*error_handler_add);
                               state.RemoveAddLineHandler(*handler_add);
                               return false;
                           });

                if (layer_count == 0) return;

                *param_layer = layer_count;
            }

            auto address = *handler_add;
            *handler_add = state.InsertAddLineHandler(count_check);
            state.RemoveAddLineHandler(address);
        });

    std::function<void(nemesis::Line&)> start_assign = [is_vector,
                                                        element_counter,
                                                        param_layer,
                                                        guid_ptr,
                                                        handler_add,
                                                        error_handler_add,
                                                        start_ptr,
                                                        first_check,
                                                        &state](nemesis::Line& line)
    {
        *start_ptr = &line;

        int layer_count = *param_layer;
        TrackLayer(layer_count,
                   line,
                   "<hkparam ",
                   "</hkparam>",
                   [is_vector, start_ptr, guid_ptr, element_counter, handler_add, error_handler_add, &state]()
                   {
                       size_t element = *element_counter / (is_vector ? 4 : 1);
                       (*start_ptr)->replace(*guid_ptr, std::to_string(element));

                       state.RemoveAddLineHandler(*handler_add);
                       state.RemoveEOFHandler(*error_handler_add);
                       return false;
                   });

        if (layer_count == 0) return;

        *param_layer = layer_count;
        auto address = *handler_add;
        *handler_add = state.InsertAddLineHandler(*first_check);
        state.RemoveAddLineHandler(address);
    };

    *handler_add       = state.InsertAddLineHandler(start_assign);
    *error_handler_add = state.InsertEOFHandler(
        [this]()
        {
            throw nemesis::ModifierException("Syntax Error: Unclose counter (Line: " + std::to_string(LineNum)
                                             + ", File: " + nemesis::to_utf8_string(FilePath) + ")");
        });
}
