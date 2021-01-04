#include <iostream>
#include <string>
#include <vector>

#define AVOID_OVERFLOW(x)       \
static_cast<                    \
    std::basic_string<          \
        char,                   \
        std::char_traits<char>, \
        std::allocator<char>    \
        >::size_type            \
    >(x)                        \

std::vector<int32_t> make_table(
    const std::string& source
)
{
    const auto size = source.length();

    int32_t i = 2;
    int32_t j = 0;

    std::vector<int32_t> result(size + 1);
    result[0] = -1;

    while (i <= size)
    {
        if (source[AVOID_OVERFLOW(i) - 1] == source[j])
        {
            result[i] = j + 1;
            i++; j++;
        }
        else if (j > 0)
        {
            j = result[j];
        }
        else
        {
            result[i] = 0;
            i++;
        }
    }

    return result;
}

bool perform_kmp(
    const std::string& source,
    const std::string& word,
    std::vector<uint32_t>& result
)
{
    if (source.empty() || word.empty())
    {
        return false;
    }

    const auto table = make_table(word);

    int32_t i = 0;
    int32_t j = 0;

    const size_t n = source.length();
    const size_t m = word.length();

    while ((static_cast<size_t>(i) + j) < n)
    {
        if (word[j] == source[AVOID_OVERFLOW(i) + j])
        {
            j++;
            if (j == m)
            {
                result.push_back(i);
                i += j - table[j];
                j = table[j];
            }
        }
        else
        {
            i += j - table[j];
            if (j > 0)
            {
                j = table[j];
            }
        }
    }

    return result.size() > 0;
}

int main(int argc, const char** argv, const char** envp)
{
    if (!(argc >= 3))
    {
        printf("invalid input\n");
        return EXIT_FAILURE;
    }

    std::vector<uint32_t> matches;
    const auto result = perform_kmp(argv[1], argv[2], matches);

    if (result)
    {
        printf("there are %zu match(es) found: \n", matches.size());

        for (auto i = 0; i < matches.size(); i++)
        {
            printf("at %d\n", matches[i]);
        }
    }
    else
    {
        printf("no matches found\n");
    }

    return EXIT_SUCCESS;
}