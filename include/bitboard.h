#ifndef BITBOARD_H
#define BITBOARD_H

#include <cstdint>
#include <iostream>

// Bitboard
class CBoard {
    // Array of pieces defined in the order of the enum
    uint64_t pieceBB[16];

#define get_bit(bitboard, square) ((bitboard >> square) & 1ULL)

  public:
    // Enumerate pieces
    enum Board {
        empty,
        white,
        black,
        whitePawn,
        whiteRook,
        whiteKnight,
        whiteBishop,
        whiteQueen,
        whiteKing,
        blackPawn,
        blackRook,
        blackKnight,
        blackBishop,
        blackQueen,
        blackKing,
        occupied
    };

    // Enumerate squares
    // clang-format off
    enum { a8, b8, c8, d8, e8, f8, g8, h8,
           a7, b7, c7, d7, e7, f7, g7, h7,
           a6, b6, c6, d6, e6, f6, g6, h6,
           a5, b5, c5, d5, e5, f5, g5, h5,
           a4, b4, c4, d4, e4, f4, g4, h4,
           a3, b3, c3, d3, e3, f3, g3, h3,
           a2, b2, c2, d2, e2, f2, g2, h2,
           a1, b1, c1, d1, e1, f1, g1, h1 };
    // clang-format -n

    // Returns the board for a certain piece type
    uint64_t getPieceSet(Board board) { return pieceBB[board]; }

    // Print the bitboad for a specific piece
    void print_bitboard(Board board) {
        std::cout << "\n  ---------------------------------\n";

        for (int rank = 0; rank < 8; rank++) {
            for (int file = 0; file < 8; file++) {

                // Print files
                if (!file) {
                    std::cout << 8 - rank << " | ";
                }

                int square = rank * 8 + file;
                // Print 1 if piece is in location else 0
                std::cout << (get_bit(pieceBB[board], square)) << " | ";
            }
            std::cout << "\n  ---------------------------------\n";
        }

        // Print ranks
        std::cout << "    a   b   c   d   e   f   g   h";

        std::cout << "\n\n    Bitboard: " << pieceBB[board] << "\n\n";
    }
};

#endif