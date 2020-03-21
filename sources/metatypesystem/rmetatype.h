#ifndef RIALIZER_RMETATYPE_H
#define RIALIZER_RMETATYPE_H

#include <QtCore/QMetaType>
#include <QtCore/QVariant>

#include <array>
#include <memory>

namespace Rializer {

// todo (Qt 6): remove, replace by QMetaType
// In Qt 6 QMetaType will:
// not require registration;
// be moveable;
// have convenient non-static members.
class RMetaType
{
public:
    bool isValid() const;
    int id() const;
    QMetaType::TypeFlags flags() const;
    const QMetaObject * metaObject() const;
    const char * name() const;

    template<typename T>
    static RMetaType fromType();

    explicit RMetaType(int typeId);
    RMetaType() = default;
    RMetaType(const RMetaType &other);
    RMetaType & operator=(const RMetaType &other);
    RMetaType(RMetaType &&) = default;
    RMetaType & operator=(RMetaType &&) = default;

protected:
    int id1;
    std::unique_ptr<QMetaType> metaType;
};

QVariant makeVariant(const RMetaType &metaType);
QVariant makeVariant(const RMetaType &metaType, const void *copy);

template<typename T>
QVariant makeVariantFromValue(T &&value);

template<typename T>
const T & variantValue(const QVariant &variant);
template<typename T>
T & variantValue(QVariant &variant);

namespace RializerPrivate {
namespace MetaType {
struct Defined;
struct NotDefined;
}
}

inline bool RMetaType::isValid() const
{
    return metaType->isValid();
}

inline int RMetaType::id() const
{
    return id1;
}

inline QMetaType::TypeFlags RMetaType::flags() const
{
    return metaType->flags();
}

inline const QMetaObject * RMetaType::metaObject() const
{
    return metaType->metaObject();
}

inline const char * RMetaType::name() const
{
    return QMetaType::typeName(id1);
}

inline RMetaType::RMetaType(const int typeId)
  : id1(typeId),
    metaType(new QMetaType(typeId))
{}

inline RMetaType::RMetaType(const RMetaType &other)
  : id1(other.id1),
    metaType(new QMetaType(id1))
{}

inline RMetaType & RMetaType::operator=(const RMetaType &other)
{
    id1 = other.id1;
    metaType.reset(new QMetaType(id1));
    return *this;
}

template<typename T>
RMetaType RMetaType::fromType()
{
    using namespace RializerPrivate::MetaType;

    using MetaType = typename std::conditional<
        QMetaTypeId2<T>::Defined,
        Defined, NotDefined
    >::type;

    return MetaType::template fromType<T>();
}

inline QVariant makeVariant(const RMetaType &metaType)
{
    return QVariant(metaType.id(), nullptr);
}

inline QVariant makeVariant(const RMetaType &metaType, const void * const copy)
{
    return QVariant(metaType.id(), copy);
}

template<typename T>
QVariant makeVariantFromValue(T &&value)
{
    QVariant variant(qMetaTypeId<T>(), nullptr);
    (*static_cast<T *>(variant.data())) = std::move(value);
    return variant;
}

template<typename T>
const T & variantValue(const QVariant &variant)
{
    return *static_cast<const T *>(variant.constData());
}

template<typename T>
T & variantValue(QVariant &variant)
{
    return *static_cast<T *>(variant.data());
}

namespace RializerPrivate {
namespace MetaType {

struct Defined
{
    template<typename T>
    static RMetaType fromType();
};

struct NotDefined
{
    template<typename T>
    static RMetaType fromType();
};

template<typename T>
const char * typeName();

template<typename T>
RMetaType Defined::fromType()
{
    return RMetaType(qMetaTypeId<T>());
}

template<typename T>
RMetaType NotDefined::fromType()
{
    const int typeId(qRegisterMetaType<T>(typeName<T>()));
    return RMetaType(typeId);
}

struct QTypeNormalizer
{
    char *output;
    int len = 0;
    char last = 0;

    QTypeNormalizer(char *output) : output(output), len(), last() {}

private:
    static bool is_ident_char(char s)
    {
        return ((s >= 'a' && s <= 'z') || (s >= 'A' && s <= 'Z') || (s >= '0' && s <= '9')
                || s == '_');
    }
    static bool is_space(char s) { return (s == ' ' || s == '\t' || s == '\n'); }
    static bool is_number(char s) { return s >= '0' && s <= '9'; }
    static bool starts_with_token(const char *b, const char *e, const char *token,
                                            bool msvcKw = false)
    {
        while (b != e && *token && *b == *token) {
            b++;
            token++;
        }
        if (*token)
            return false;
#ifdef Q_CC_MSVC
        /// On MSVC, keywords like class or struct are not separated with spaces in constexpr
        /// context
        if (msvcKw)
            return true;
#endif
        Q_UNUSED(msvcKw)
        return b == e || !is_ident_char(*b);
    }
    static bool skipToken(const char *&x, const char *e, const char *token,
                                    bool msvcKw = false)
    {
        if (!starts_with_token(x, e, token, msvcKw))
            return false;
        while (*token++)
            x++;
        while (x != e && is_space(*x))
            x++;
        return true;
    }
    static const char *skipString(const char *x, const char *e)
    {
        char delim = *x;
        x++;
        while (x != e && *x != delim) {
            if (*x == '\\') {
                x++;
                if (x == e)
                    return e;
            }
            x++;
        }
        if (x != e)
            x++;
        return x;
    }
    static const char *skipTemplate(const char *x, const char *e, bool stopAtComa = false)
    {
        int scopeDepth = 0;
        int templateDepth = 0;
        while (x != e) {
            switch (*x) {
            case '<':
                if (!scopeDepth)
                    templateDepth++;
                break;
            case ',':
                if (stopAtComa && !scopeDepth && !templateDepth)
                    return x;
                break;
            case '>':
                if (!scopeDepth)
                    if (--templateDepth < 0)
                        return x;
                break;
            case '(':
            case '[':
            case '{':
                scopeDepth++;
                break;
            case '}':
            case ']':
            case ')':
                scopeDepth--;
                break;
            case '\'':
                if (is_number(x[-1]))
                    break;
                Q_FALLTHROUGH();
            case '\"':
                x = skipString(x, e);
                continue;
            }
            x++;
        }
        return x;
    }

    void append(char x)
    {
        last = x;
        len++;
        if (output)
            *output++ = x;
    }

    void appendStr(const char *x)
    {
        while (*x)
            append(*x++);
    }

public:
    int normalizeType(const char *begin, const char *end, bool adjustConst = true)
    {
        // Trim spaces
        while (begin != end && is_space(*begin))
            begin++;
        while (begin != end && is_space(*(end - 1)))
            end--;

        // Convert 'char const *' into 'const char *'. Start at index 1,
        // not 0, because 'const char *' is already OK.
        const char *cst = begin + 1;
        if (*begin == '\'' || *begin == '"')
            cst = skipString(begin, end);
        bool seenStar = false;
        bool hasMiddleConst = false;
        while (cst < end) {
            if (*cst == '\"' || (*cst == '\'' && !is_number(cst[-1]))) {
                cst = skipString(cst, end);
                if (cst == end)
                    break;
            }

            // We mustn't convert 'char * const *' into 'const char **'
            // and we must beware of 'Bar<const Bla>'.
            if (*cst == '&' || *cst == '*' || *cst == '[') {
                seenStar = *cst != '&' || cst != (end - 1);
                break;
            }
            if (*cst == '<') {
                cst = skipTemplate(cst + 1, end);
                if (cst == end)
                    break;
            }
            cst++;
            const char *skipedCst = cst;
            if (!is_ident_char(*(cst - 1)) && skipToken(skipedCst, end, "const")) {
                const char *testEnd = end;
                while (skipedCst < testEnd--) {
                    if (*testEnd == '*' || *testEnd == '['
                        || (*testEnd == '&' && testEnd != (end - 1))) {
                        seenStar = true;
                        break;
                    }
                    if (*testEnd == '>')
                        break;
                }
                if (adjustConst && !seenStar) {
                    if (*(end - 1) == '&')
                        end--;
                } else {
                    appendStr("const ");
                }
                normalizeType(begin, cst, false);
                begin = skipedCst;
                hasMiddleConst = true;
                break;
            }
        }
        if (skipToken(begin, end, "const")) {
            if (adjustConst && !seenStar) {
                if (*(end - 1) == '&')
                    end--;
            } else {
                appendStr("const ");
            }
        }
        if (seenStar && adjustConst) {
            const char *e = end;
            if (*(end - 1) == '&' && *(end - 2) != '&')
                e--;
            while (begin != e && is_space(*(e - 1)))
                e--;
            const char *token = "tsnoc"; // 'const' reverse, to check if it ends with const
            while (*token && begin != e && *(--e) == *token++)
                ;
            if (!*token && begin != e && !is_ident_char(*(e - 1))) {
                while (begin != e && is_space(*(e - 1)))
                    e--;
                end = e;
            }
        }

        // discard 'struct', 'class', and 'enum'; they are optional
        // and we don't want them in the normalized signature
        skipToken(begin, end, "struct", true) || skipToken(begin, end, "class", true)
                || skipToken(begin, end, "enum", true);

#ifdef QT_NAMESPACE
        const char *nsbeg = begin;
        if (skipToken(nsbeg, end, QT_STRINGIFY(QT_NAMESPACE)) && nsbeg + 2 < end && nsbeg[0] == ':'
            && nsbeg[1] == ':') {
            begin = nsbeg + 2;
            while (begin != end && is_space(*begin))
                begin++;
        }
#endif

        if (skipToken(begin, end, "QList")) {
            // Replace QList by QVector
            appendStr("QVector");
        }
        if (!hasMiddleConst) {
            // Normalize the integer types
            int numLong = 0;
            int numSigned = 0;
            int numUnsigned = 0;
            int numInt = 0;
            int numShort = 0;
            int numChar = 0;
            while (begin < end) {
                if (skipToken(begin, end, "long")) {
                    numLong++;
                    continue;
                }
                if (skipToken(begin, end, "int")) {
                    numInt++;
                    continue;
                }
                if (skipToken(begin, end, "short")) {
                    numShort++;
                    continue;
                }
                if (skipToken(begin, end, "unsigned")) {
                    numUnsigned++;
                    continue;
                }
                if (skipToken(begin, end, "signed")) {
                    numSigned++;
                    continue;
                }
                if (skipToken(begin, end, "char")) {
                    numChar++;
                    continue;
                }
                break;
            }
            if (numChar || numShort) {
                if (numSigned && numChar)
                    appendStr("signed ");
                if (numUnsigned)
                    appendStr("unsigned ");
                if (numChar)
                    appendStr("char");
                else
                    appendStr("short");
            } else if (numLong) {
                if (numLong == 1) {
                    if (numUnsigned)
                        append('u');
                    appendStr("long");
                } else {
                    if (numUnsigned)
                        appendStr("unsigned ");
                    appendStr("long long");
                }
            } else if (numUnsigned || numSigned || numInt) {
                if (numUnsigned)
                    append('u');
                appendStr("int");
            }
        }

        bool spaceSkiped = true;
        while (begin != end) {
            char c = *begin++;
            if (is_space(c)) {
                spaceSkiped = true;
            } else if ((c == '\'' && !is_number(last)) || c == '\"') {
                begin--;
                auto x = skipString(begin, end);
                while (begin < x)
                    append(*begin++);
            } else {
                if (spaceSkiped && is_ident_char(last) && is_ident_char(c))
                    append(' ');
                append(c);
                spaceSkiped = false;
                if (c == '<') {
                    do {
                        // template recursion
                        const char *tpl = skipTemplate(begin, end, true);
                        normalizeType(begin, tpl, false);
                        if (tpl == end)
                            return len;
                        append(*tpl);
                        begin = tpl;
                    } while (*begin++ == ',');
                }
            }
        }
        return len;
    }
};

inline int qNormalizeType(const char *begin, const char *end, char *output)
{
    return QTypeNormalizer(output).normalizeType(begin, end);
}

template<typename T>
const char * typeName()
{
    constexpr auto prefix = sizeof(
#ifdef QT_NAMESPACE
        QT_STRINGIFY(QT_NAMESPACE) "::"
#endif
#ifdef Q_CC_MSVC
        "const char *__cdecl Rializer::RializerPrivate::MetaType::typeName<"
#elif defined(Q_CC_CLANG)
        "const char* Rializer::RializerPrivate::MetaType::typeName() [T = "
#else
        "const char* Rializer::RializerPrivate::MetaType::typeName() [with T = "
#endif
    ) - 1;
#ifdef Q_CC_MSVC
    constexpr auto suffix = sizeof(">(void)");
#else
    constexpr auto suffix = sizeof("]");
#endif
    constexpr auto len1 = sizeof(Q_FUNC_INFO);

    auto func = Q_FUNC_INFO;
    const char *begin = func + prefix;
    const char *end = func + len1 - suffix;
    constexpr auto len = len1 - suffix - prefix;

    static std::array<char, len + 1> result {};
    qNormalizeType(begin, end, result.data());
    return result.data();
}

} // namespace MetaType
} // namespace RializerPrivate

} // namespace Rializer

#endif // RIALIZER_RMETATYPE_H
