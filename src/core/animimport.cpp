#include "core/animimport.h"

#include "utilities/template.h"

nemesis::AnimImport::AnimImport(const std::string& id)
{
    this->id = id;
    parameters.emplace_back(id);
}

void nemesis::AnimImport::AddParameter(const std::string& param)
{
    parameters.emplace_back(param);
}

void nemesis::AnimImport::AddImportTemplate(const nemesis::Template* templt_ptr)
{
    this->templt_ptr = templt_ptr;
}

std::string_view nemesis::AnimImport::GetID() const
{
    return id;
}

const VecStr& nemesis::AnimImport::GetParameters() const
{
    return parameters;
}

std::string_view nemesis::AnimImport::GetParameter(size_t index) const
{
    return parameters.at(index);
}

void nemesis::AnimImport::GetLines(VecNstr& buffer, nemesis::Exporter& exptr)
{
    templt_ptr->GetImportResult(buffer, exptr);
}
