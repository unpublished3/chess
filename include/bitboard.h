#ifndef BITBOARD_H
#define BITBOARD_H

#include <cmath>
#include <cstdint>
#include <iostream>

// Bitboard
class CBoard {
    // Array of pieces defined in the order of the enum
    uint64_t pieceBB[16];

  public:
    CBoard() { initialize(); }

    /*****************************************************************
     =================================================================
                                    Enums
     =================================================================
     *****************************************************************/

    // Enumerate pieces
    enum Board {
        empty,
        white,
        black,
        whitePawns,
        whiteRooks,
        whiteKnights,
        whiteBishops,
        whiteQueens,
        whiteKing,
        blackPawns,
        blackRooks,
        blackKnights,
        blackBishops,
        blackQueens,
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
    // clang-format on

    /****************************************************************
     ================================================================
                                Board
     ================================================================
     ****************************************************************/


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

    /****************************************************************
     ================================================================
                            Bit Manipulation
     ================================================================
     ****************************************************************/

    // Set initial position for all the pieces on the board
    void initialize() {
        // Pawns
        pieceBB[whitePawns] = 0x00FF000000000000ULL;
        pieceBB[blackPawns] = 0x000000000000FF00ULL;

        // Rooks
        pieceBB[whiteRooks] = 0x8100000000000000ULL;
        pieceBB[blackRooks] = 0x0000000000000081ULL;

        // Knigts
        pieceBB[whiteKnights] = 0x4200000000000000ULL;
        pieceBB[blackKnights] = 0x0000000000000042ULL;

        // // Bishops
        pieceBB[whiteBishops] = 0x2400000000000000ULL;
        pieceBB[blackBishops] = 0x0000000000000024ULL;

        // // Queens
        pieceBB[whiteQueens] = 0x0800000000000000ULL;
        pieceBB[blackQueens] = 0x0000000000000008ULL;

        // // King
        pieceBB[whiteKing] = 0x1000000000000000ULL;
        pieceBB[blackKing] = 0x0000000000000010ULL;

        // // Combined
        pieceBB[white] = pieceBB[whitePawns] | pieceBB[whiteRooks] |
                         pieceBB[whiteKnights] | pieceBB[whiteBishops] |
                         pieceBB[whiteQueens] | pieceBB[whiteKing];

        pieceBB[black] = pieceBB[blackPawns] | pieceBB[blackRooks] |
                         pieceBB[blackKnights] | pieceBB[blackBishops] |
                         pieceBB[blackQueens] | pieceBB[blackKing];
        ;
        pieceBB[occupied] = pieceBB[white] | pieceBB[black];
    }

    // Get bit at specific positon for a bitboard
    int get_bit(uint64_t bitboard, int square) {
        return (bitboard >> square) & 1ULL;
    }

    // Set a specific bit for a bitboard
    void set_bit(Board board, int square) {
        pieceBB[board] |= (1ULL << square);
    }

    // Pop a specific bit for a bitboard
    void pop_bit(Board board, int square) {
        pieceBB[board] &= ~(1ULL << square);
    }

    /****************************************************************
     ================================================================
                                Attacks
     ================================================================
     ****************************************************************/

};

#endif
