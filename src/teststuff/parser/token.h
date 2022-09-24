#ifndef TOKEN_H
#define TOKEN_H

enum class TokenType {
    String,
    Literal,
    Number,
    Name,
    Punctuation
};

enum class TokenSubtype {
    Integer,
    Decimal,
    Hex,
    Octal,
    Binary,
    Long,
    Unsigned,
    Float,
    SinglePrecision,
    DoublePrecision,
    ExtendedPrecision,
    Infinite,
    Indefinite,
    NotANumber,
    IPAddress,
    IPPort
};

#endif /* TOKEN_H */
