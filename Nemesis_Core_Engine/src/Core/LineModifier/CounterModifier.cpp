#include <iomanip>
#include <random>
#include <sstream>

#include "Core/LineModifier/CounterModifier.h"

#include "Core/CompileState.h"

#include "Utilities/Algorithm.h"

std::string nemesis::CounterModifier::GenerateGuid()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 15);

    std::stringstream ss;
    ss << std::hex;

    for (int i = 0; i < 32; ++i)
    {
        int rand_num = dis(gen);
        ss << (i == 8 || i == 12 || i == 16 || i == 20 ? "-" : "") << rand_num;
    }

    return ss.str();
}

nemesis::CounterModifier::CounterModifier(size_t begin,
                                          size_t end,
                                          const std::string& name,
                                          size_t linenum,
                                          const std::filesystem::path& filepath) noexcept
    : nemesis::LineModifier(begin, end)
    , CounterName(name)
    , AddCounterSyntax("<!-- ADD_COUNTER_" + name + " -->")
    , EndCounterSyntax("<!-- END_COUNTER_" + name + " -->")
    , StartCounterRegex("\\s*\\<\\!-- START_COUNTER_" + name + " --\\>")
    , AddCounterRegex("\\s*\\<\\!-- ADD_COUNTER_" + name + " --\\>")
    , EndCounterRegex("\\s*\\<\\!-- END_COUNTER_" + name + " --\\>")
    , LineNum(linenum)
    , FilePath(filepath)
{
}

void nemesis::CounterModifier::Apply(VecStr& blocks, nemesis::CompileState& state) const
{
    auto element_counter   = std::make_shared<int>(0);
    auto handler_add       = std::make_shared<void*>(nullptr);
    auto error_handler_add = std::make_shared<std::function<void()>*>(nullptr);

    auto start_ptr = std::make_shared<nemesis::Line*>(nullptr);
    auto guid_ptr  = std::make_shared<std::string>("{" + GenerateGuid() + "}");

    ClearCoveredBlocks(blocks);
    blocks[Begin] = *guid_ptr;

    state.AddCounter(CounterName, element_counter.get());

    std::function<void(nemesis::Line&)> counter_callback
        = [this, element_counter, handler_add, error_handler_add, start_ptr, guid_ptr, &state](
              nemesis::Line& line)
    {
        int& elem_counter = *element_counter;

        if (line.find(AddCounterSyntax) != NOT_FOUND)
        {
            elem_counter++;
            line = std::regex_replace(line.ToString(), AddCounterRegex, "");
            return;
        }

        if (line.find(EndCounterSyntax) != NOT_FOUND)
        {
            line = std::regex_replace(line.ToString(), EndCounterRegex, "");
            (*start_ptr)->replace(*guid_ptr, std::to_string(elem_counter));

            state.RemoveEOFHandler(*error_handler_add);
            state.RemoveAddLineHandler(*handler_add);
        }
    };

    *handler_add = state.InsertAddLineHandler(
        [this, handler_add, start_ptr, counter_callback, &state](nemesis::Line& line)
        {
            line         = std::regex_replace(line.ToString(), StartCounterRegex, "");
            (*start_ptr) = &line;

            auto address   = *handler_add;
            (*handler_add) = state.InsertAddLineHandler(counter_callback);
            state.RemoveAddLineHandler(address);
        });

    *error_handler_add = state.InsertEOFHandler(
        [this]()
        {
            throw std::runtime_error("Syntax Error: Unclose counter (Counter name: " + CounterName
                                     + ", Line: " + std::to_string(LineNum)
                                     + ", File: " + nemesis::to_utf8_string(FilePath) + ")");
        });
}
