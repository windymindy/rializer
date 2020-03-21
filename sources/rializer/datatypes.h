#ifndef RIALIZER_DATATYPES_H
#define RIALIZER_DATATYPES_H

#include <QtCore/QLoggingCategory>

namespace Rializer {

struct Mode
{
    bool serialize;
    bool deserialize;

    Mode();
};

struct Options
{
    bool noHumanReadableFormat;

    Options();

    Q_GADGET
    Q_PROPERTY(bool noHumanReadableFormat MEMBER noHumanReadableFormat)
};

struct SourceLocation
{
    int line;
    const char *file;
    const char *function;

    // todo: can not return QMessageLogContext
    // operator QMessageLogContext() const;
    void toContext(QMessageLogContext *context) const;

    SourceLocation(
        const char *file,
        int line,
        const char *function
    );
    SourceLocation() = default;
};

#define RIALIZER_SOURCELOCATION \
    SourceLocation( \
        QT_MESSAGELOG_FILE, \
        QT_MESSAGELOG_LINE, \
        QT_MESSAGELOG_FUNC \
    )

struct TextMessage
{
    QLatin1String id;
    SourceLocation location;
    const QLoggingCategory & category() const;
    QString textTemplate;
    std::vector<QString> arguments;
    QString text() const;

    bool isNull() const;
    void clear();

    TextMessage(
        const char *id,
        const char *textTemplate,
        std::vector<QString> &&arguments,
        SourceLocation location
    );
    TextMessage(
        const char *id,
        const char *textTemplate,
        SourceLocation location
    );
    TextMessage() = default;
};

// Q_DECLARE_LOGGING_CATEGORY(loggingCategory)
const QLoggingCategory & loggingCategory();

enum struct Reason
{
    None,
    TypeIsNotValid,
    ValueIsNotValid,
    RepresentationIsNotValid
};

struct Error
{
    Reason reason;
    TextMessage message;

    bool isNull() const;
    void clear();

    Error(Reason reason, TextMessage &&message);
    Error() = default;
};

struct Data
{
    Mode mode;
    Options options;
    Error error;

    explicit Data(Options &&options);
    Data() = default;
};

inline Mode::Mode()
  : serialize(false),
    deserialize(false)
{}

inline Options::Options()
  : noHumanReadableFormat(false)
{}

inline const QLoggingCategory & TextMessage::category() const
{
    return loggingCategory();
}

inline QString TextMessage::text() const
{
    QString text(textTemplate);
    for (const auto &argument : arguments)
        text = text.arg(argument);
    return text;
}

inline bool TextMessage::isNull() const
{
    return id.isEmpty();
}

inline void TextMessage::clear()
{
    *this = TextMessage();
}

inline TextMessage::TextMessage(
    const char * const id,
    const char * const textTemplate,
    std::vector<QString> &&arguments,
    const SourceLocation location
) : id(id),
    location(location),
    textTemplate(textTemplate),
    arguments(std::move(arguments))
{}

inline TextMessage::TextMessage(
    const char * const id,
    const char * const textTemplate,
    const SourceLocation location
) : id(id),
    location(location),
    textTemplate(textTemplate),
    arguments()
{}

/*
inline SourceLocation::operator QMessageLogContext() const
{
    return QMessageLogContext(file, line, function, loggingCategory().categoryName());
}
*/

inline void SourceLocation::toContext(QMessageLogContext * const context) const
{
    context->file = file;
    context->line = line;
    context->function = function;
    context->category = loggingCategory().categoryName();
}

inline SourceLocation::SourceLocation(
    const char * const file,
    const int line,
    const char * const function
) : line(line),
    file(file),
    function(function)
{}

inline bool Error::isNull() const
{
    return reason == Reason::None;
}

inline void Error::clear()
{
    *this = Error();
}

inline Error::Error(const Reason reason, TextMessage &&message)
  : reason(reason),
    message(std::move(message))
{}

inline Data::Data(Options &&options)
  : mode(),
    options(std::move(options)),
    error()
{}

// Q_LOGGING_CATEGORY(loggingCategory, "rializer")
inline const QLoggingCategory & loggingCategory()
{
    // This is not exported.
    // Each shared object or application has exactly one instance.
    static const QLoggingCategory category("rializer");
    return category;
}

} // namespace Rializer;

#endif // RIALIZER_DATATYPES_H
