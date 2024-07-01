#ifndef BITBOARD_H
#define BITBOARD_H

#include <cstdint>
#include <cstdio>
#include <iostream>
#include <iterator>

// Bitboard
class CBoard {
    // Array of pieces defined in the order of the enum
    uint64_t pieceBB[14];
    uint64_t emptyBB = 0ULL;
    uint64_t occupiedBB;

    #define get_bit(bitboard, square) ((bitboard >> square) & 1ULL)

  public:
    enum Piece {
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
        blackKing
    };

    // Returns the board for a certain piece type
    uint64_t getPieceSet(Piece pt) { return pieceBB[pt]; }

    // Print the bitboad for a specific piece
    void print_bitboard() {
        std::cout << "\n  ---------------------------------\n";

        for (int rank = 0; rank < 8; rank++) {
            for (int file = 0; file < 8; file++) {

                // Print files
                if (!file) {
                    std::cout << 8 - rank << " | ";
                }

                int square = rank * 8 + file;
                // Print 1 if piece is in location else 0
                std::cout << (get_bit(occupiedBB, square)) << " | ";
            }
            std::cout << "\n  ---------------------------------\n";
        }

        // Print ranks
        std::cout << "    a   b   c   d   e   f   g   h";

        std::cout << "\n\n    Bitboard: " << occupiedBB << "\n\n";
    }
};

#endif