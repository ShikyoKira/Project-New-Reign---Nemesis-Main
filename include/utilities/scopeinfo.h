/*

THE DESIGN PHILOSOPHY OF SCOPE INFO
- MUST BE CONST
- CONTAINS TEMPORARY INFORMATION
- ONLY EXIST 1 AT A TIME
- LIFE TIME LAST UNTIL IT GOES OUT OF SCOPE
- EXISTS ONLY IN TEMPLATE

The idea of scope info is to store temporary information that can be used
as parameters in nemesis functions within the scope, which includes
conditions that will create another nested scope and so on

*/

namespace nemesis
{
	struct ScopeInfo
    {
    private:
        bool isGroup;
        bool isMaster;
        bool negative;
        bool isEnd;
        bool elementCatch;
        bool norElement;

        int counter = 0;

    public:
        ScopeInfo(bool _isGroup, bool _isMaster);
        uint GetOptionIndex(const std::string option) const;

    };
}
