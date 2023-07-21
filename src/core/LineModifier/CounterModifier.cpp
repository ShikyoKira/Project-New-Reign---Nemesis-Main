#include <random>
#include <sstream>
#include <iomanip>

#include "core/LineModifier/CounterModifier.h"

#include "core/CompileState.h"

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
    , LineNum(linenum)
    , FilePath(filepath)
    , CounterName(name)
    , AddCounterSyntax("<!-- ADD_COUNTER_" + name + " -->")
    , EndCounterSyntax("<!-- END_COUNTER_" + name + " -->")
    , StartCounterRegex("\\s*\\<\\!-- START_COUNTER_" + name + " --\\>")
    , AddCounterRegex("\\s*\\<\\!-- ADD_COUNTER_" + name + " --\\>")
    , EndCounterRegex("\\s*\\<\\!-- END_COUNTER_" + name + " --\\>")
{
}

void nemesis::CounterModifier::Apply(VecStr& blocks, nemesis::CompileState& state) const 
{
    SPtr<int> element_counter         = std::make_shared<int>(0);
    SPtr<uintptr_t> handler_add       = std::make_shared<uintptr_t>(0);
    SPtr<uintptr_t> error_handler_add = std::make_shared<uintptr_t>(0);

    SPtr<nemesis::Line*> start_ptr = std::make_shared<nemesis::Line*>(nullptr);
    SPtr<std::string> guid_ptr     = std::make_shared<std::string>("{" + GenerateGuid() + "}");

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
            line = nemesis::regex_replace(line, AddCounterRegex, "");
            return;
        }

        if (line.find(EndCounterSyntax) != NOT_FOUND)
        {
            line = nemesis::regex_replace(line, EndCounterRegex, "");
            (*start_ptr)->replace(*guid_ptr, std::to_string(elem_counter));

            state.RemoveEOFHandler(*error_handler_add);
            state.RemoveAddLineHandler(*handler_add);
        }
    };

    *handler_add = state.InsertAddLineHandler(
        [this, handler_add, start_ptr, counter_callback, &state](nemesis::Line& line)
        {
            line         = nemesis::regex_replace(line, StartCounterRegex, "");
            (*start_ptr) = &line;

            auto address   = *handler_add;
            (*handler_add) = state.InsertAddLineHandler(counter_callback);
            state.RemoveAddLineHandler(address);
        });

    *error_handler_add = state.InsertEOFHandler(
        [this]()
        {
            throw std::runtime_error("Syntax error: Unclose counter (Counter name: " + CounterName
                                     + ", LineNum: " + std::to_string(LineNum)
                                     + ", FilePath: " + FilePath.string() + ")");
        });
}
