#pragma once

namespace nemesis
{
    struct Template;
    struct Exporter;

	struct AnimImport
    {
    private:
        std::string id;
        VecStr parameters;

        const nemesis::Template* templt_ptr;

    public:
        AnimImport(const std::string& id);

        void AddParameter(const std::string& param);
        void AddImportTemplate(const nemesis::Template* templt_ptr);

        std::string_view GetID() const;
        const VecStr& GetParameters() const;
        std::string_view GetParameter(size_t index) const;

        void GetLines(VecNstr& buffer, nemesis::Exporter& exptr);
    };
}
