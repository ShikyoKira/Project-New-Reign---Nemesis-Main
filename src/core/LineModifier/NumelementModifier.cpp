#include "core/LineModifier/NumelementModifier.h"

#include "core/CompileState.h"

#include "utilities/regex.h"
#include "utilities/algorithm.h"

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

bool TrackLayer(int& layer, const std::string& text, const std::string& open, const std::string& close, std::function<bool()> continue_callback)
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

nemesis::NumelementModifier::NumelementModifier(size_t begin,
                                                size_t end,
                                                size_t linenum,
                                                const std::filesystem::path& filepath)
    : nemesis::LineModifier(begin, end)
    , LineNum(linenum)
    , FilePath(filepath)
{
}

void nemesis::NumelementModifier::Apply(VecStr& blocks, nemesis::CompileState& state) const
{
    SPtr<int> element_counter = std::make_shared<int>(0);
    SPtr<int> param_layer     = std::make_shared<int>(0);
    SPtr<int> object_layer    = std::make_shared<int>(0);

    SPtr<uintptr_t> handler_add       = std::make_shared<uintptr_t>(0);
    SPtr<uintptr_t> error_handler_add = std::make_shared<uintptr_t>(0);

    SPtr<nemesis::Line*> start_ptr = std::make_shared<nemesis::Line*>(nullptr);
    SPtr<std::string> guid_ptr     = std::make_shared<std::string>("{" + nemesis::generate_guid() + "}");

    ClearCoveredBlocks(blocks);
    blocks[Begin] = *guid_ptr;

    SPtr<std::function<void(nemesis::Line&)>> counting_handler
        = std::make_shared<std::function<void(nemesis::Line&)>>([](nemesis::Line&) {});

    std::function<void(nemesis::Line&)> count_check = [element_counter,
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
            int& layer_count = *param_layer;
            auto cont
                = TrackLayer(layer_count,
                             line,
                             "<hkparam ",
                             "</hkparam>",
                             [start_ptr, guid_ptr, element_counter, handler_add, error_handler_add, &state]()
                             {
                                 (*start_ptr)->replace(*guid_ptr, std::to_string(*element_counter));

                                 state.RemoveAddLineHandler(*handler_add);
                                 state.RemoveEOFHandler(*error_handler_add);
                                 return false;
                             });

            if (!cont) return;
        }

        (*counting_handler)(line);
    };

    std::function<void(nemesis::Line&)> first_check = [element_counter,
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
        if (line.find("<hkobject>") != NOT_FOUND)
        {
            *counting_handler = [element_counter, object_layer](nemesis::Line& line)
            {
                int& layer_count = *object_layer;
                TrackLayer(layer_count,
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
        else if(line.find("(") != NOT_FOUND)
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
                std::istringstream iss(line);
    
                while (iss >> e)
                {
                    (*element_counter)++;
                }
            };
        }

        (*counting_handler)(line);

        if (*object_layer == 0)
        {
            int& layer_count = *param_layer;
            TrackLayer(layer_count,
                       line,
                       "<hkparam ",
                       "</hkparam>",
                       [start_ptr, guid_ptr, element_counter, handler_add, error_handler_add, &state]()
                       {
                           (*start_ptr)->replace(*guid_ptr, std::to_string(*element_counter));

                           state.RemoveAddLineHandler(*handler_add);
                           state.RemoveEOFHandler(*error_handler_add);
                           return false;
                       });

            if (layer_count == 0) return;
        }

        state.RemoveAddLineHandler(*handler_add);
        *handler_add = state.InsertAddLineHandler(count_check);
    };

    std::function<void(nemesis::Line&)> start_assign = [element_counter,
                                                        param_layer,
                                                        guid_ptr,
                                                        handler_add,
                                                        error_handler_add,
                                                        start_ptr,
                                                        first_check,
                                                        &state](nemesis::Line& line)
    {
        *start_ptr = &line;

        int& layer_count = *param_layer;
        TrackLayer(layer_count,
                   line,
                   "<hkparam ",
                   "</hkparam>",
                   [start_ptr, guid_ptr, element_counter, handler_add, error_handler_add, &state]()
                   {
                       (*start_ptr)->replace(*guid_ptr, std::to_string(*element_counter));

                       state.RemoveAddLineHandler(*handler_add);
                       state.RemoveEOFHandler(*error_handler_add);
                       return false;
                   });

        if (layer_count == 0) return;

        state.RemoveAddLineHandler(*handler_add);
        *handler_add = state.InsertAddLineHandler(first_check);
    };

    *handler_add = state.InsertAddLineHandler(start_assign);

    *error_handler_add = state.InsertEOFHandler(
        [this]()
        {
            throw std::runtime_error("Syntax error: Unclose counter (LineNum: " + std::to_string(LineNum)
                                     + ", FilePath: " + FilePath.string() + ")");
        });
}
