#include "utilities/linkedparser.h"

template <typename T>
void nemesis::LinkedParser<T>::ParseLine(const nemesis::Line& line)
{
    to_parse(line);
}

template <typename T>
void nemesis::LinkedParser<T>::SetTypeCondition(std::function<bool(const nemesis::Line&)> func)
{
    typeconditions.emplace_back(func);
    StateValidation();
}

template <typename T>
void nemesis::LinkedParser<T>::SetTypeRef(std::function<void(nemesis::LinkedVar<std::string>&)> func)
{
    typerefs.emplace_back(func);
    StateValidation();
}

template <typename T>
nemesis::LinkedParser<T> nemesis::LinkedParser<T>::CreateParser(Vec<nemesis::LinkedVar<T>>& list,
                                                                const std::string& modcode,
                                                                const std::filesystem::path& path)
{
    LinkedParser<T> parser;
    parser.stream.emplace_back(&list);
    parser.cscope = std::make_unique<ConditionScope>(modcode, path);
    return parser;
}
