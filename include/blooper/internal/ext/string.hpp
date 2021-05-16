#ifndef BLOOPER_EXT_STRING_HPP
#define BLOOPER_EXT_STRING_HPP


BLOOPER_EXT_NAMESPACE_BEGIN

namespace string
{
namespace
{
constexpr inline auto pathSeparator = '/';


inline auto split(
        const juce::String& string,
        juce::juce_wchar    by)
{
    juce::StringArray result;

    auto end = string.getCharPointer();
    auto begin = end;

    while (!end.isEmpty())
    {
        if (*end == by)
        {
            result.add({begin, end});
            begin = ++end;
        }
        else
        {
            ++end;
        }
    }
    result.add({begin, end});

    return result;
}


inline auto splitPath(const juce::String& path)
{
    return split(path, pathSeparator);
}
} // namespace
} // namespace string

BLOOPER_EXT_NAMESPACE_END


#endif // BLOOPER_EXT_STRING_HPP
