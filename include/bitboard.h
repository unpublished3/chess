#ifndef BITBOARD_H
#define BITBOARD_H

#include <array>
#include <cmath>
#include <cstdint>
#include <iostream>
#include <optional>
#include <sys/types.h>

#define count_bits(bitboard) __builtin_popcountll(bitboard);

// Bitboard
class CBoard {
    // Array of pieces defined in the order of the enum
    uint64_t pieceBB[16];
    uint64_t attackTable[8][64];

    // Values where all bits are set except for specific positions
    // For generating pawn moves
    const uint64_t set_except_a_file = 18374403900871474942ULL;
    const uint64_t set_except_h_file = 9187201950435737471ULL;

    // For knight moves
    const uint64_t set_except_gh_file = 4557430888798830399ULL;
    const uint64_t set_except_ab_file = 18229723555195321596ULL;

    /****************************************************************
     ================================================================
                                Attacks
     ================================================================
     ****************************************************************/

    // Pawn attack table
    uint64_t mask_pawn_attacks(int side, int square) {
        // Attacks bitboard
        uint64_t attacks = 0ULL;

        // Single piece bitboard
        uint64_t bitboard = 0ULL;
        set_bit(bitboard, square);

        // White pawns
        if (!side) {
            attacks |= ((bitboard >> 7) & set_except_a_file);
            attacks |= ((bitboard >> 9) & set_except_h_file);
        }
        // Black Pawns
        else {
            attacks |= ((bitboard << 7) & set_except_h_file);
            attacks |= ((bitboard << 9) & set_except_a_file);
        }

        return attacks;
    }

    // Knight attack table
    uint64_t mask_knight_attacks(int square) {
        // Attacks bitboard
        uint64_t attacks = 0ULL;

        // Single piece bitboard
        uint64_t bitboard = 0ULL;
        set_bit(bitboard, square);

        // Move up from knight
        // Two steps up
        attacks |= ((bitboard >> 17) & set_except_h_file);
        attacks |= ((bitboard >> 15) & set_except_a_file);

        // One step up
        attacks |= ((bitboard >> 10) & set_except_gh_file);
        attacks |= ((bitboard >> 6) & set_except_ab_file);

        // Move down from knight
        // Two steps down
        attacks |= ((bitboard << 17) & set_except_a_file);
        attacks |= ((bitboard << 15) & set_except_h_file);

        // One step down
        attacks |= ((bitboard << 10) & set_except_ab_file);
        attacks |= ((bitboard << 6) & set_except_gh_file);

        return attacks;
    }

    // King attack tables
    uint64_t mask_king_attacks(int square) {
        // Attacks bitboard
        uint64_t attacks = 0ULL;

        // Single piece bitboard
        uint64_t bitboard = 0ULL;
        set_bit(bitboard, square);

        // Up and Down
        attacks |= ((bitboard >> 8));
        attacks |= ((bitboard << 8));

        // Left and Right
        attacks |= ((bitboard >> 1) & set_except_h_file);
        attacks |= ((bitboard << 1) & set_except_a_file);

        // Diagonals
        attacks |= ((bitboard >> 9) & set_except_h_file);
        attacks |= ((bitboard >> 7) & set_except_a_file);
        attacks |= ((bitboard << 9) & set_except_a_file);
        attacks |= ((bitboard << 7) & set_except_h_file);

        return attacks;
    }

    // Initialize attakcs for king, knights and pawns
    void init_leaper_attacks() {
        for (int square = 0; square < 64; square++) {
            // Add masked attack for each square
            // Pawns
            attackTable[whitePawnsA][square] = mask_pawn_attacks(white, square);
            attackTable[blackPawnsA][square] = mask_pawn_attacks(black, square);

            // Knights
            attackTable[knightsA][square] = mask_knight_attacks(square);

            // King
            attackTable[kingA][square] = mask_king_attacks(square);
        }
    }

    // Bishop attack table
    uint64_t mask_bishop_attacks(int square) {
        // Attacks bitboard;
        uint64_t attacks = 0ULL;

        // Ranks and files for attacking
        int rank, file;
        int occupied_rank = square / 8, occupied_file = square % 8;

        // Bottom Right
        for (rank = occupied_rank + 1, file = occupied_file + 1;
             rank <= 6 && file <= 6; rank++, file++)
            attacks |= (1ULL << (rank * 8 + file));

        // Top left
        for (rank = occupied_rank - 1, file = occupied_file - 1;
             rank >= 1 && file >= 1; rank--, file--)
            attacks |= (1ULL << (rank * 8 + file));

        // Bottom left
        for (rank = occupied_rank + 1, file = occupied_file - 1;
             rank <= 6 && file >= 1; rank++, file--)
            attacks |= (1ULL << (rank * 8 + file));

        // Top right
        for (rank = occupied_rank - 1, file = occupied_file + 1;
             rank >= 1 && file <= 6; rank--, file++)
            attacks |= (1ULL << (rank * 8 + file));

        return attacks;
    }

    // Generate bishop attacks
    uint64_t generate_bishop_attacks(int square, uint64_t blockers) {
        // Attacks bitboard;
        uint64_t attacks = 0ULL;

        // Ranks and files for attacking
        int rank, file;
        int occupied_rank = square / 8, occupied_file = square % 8;

        // Bottom Right
        for (rank = occupied_rank + 1, file = occupied_file + 1;
             rank <= 7 && file <= 7; rank++, file++) {
            attacks |= (1ULL << (rank * 8 + file));
            if ((1ULL << (rank * 8 + file)) & blockers)
                break;
        }

        // Top left
        for (rank = occupied_rank - 1, file = occupied_file - 1;
             rank >= 0 && file >= 0; rank--, file--) {
            attacks |= (1ULL << (rank * 8 + file));
            if ((1ULL << (rank * 8 + file)) & blockers)
                break;
        }
        // Bottom left
        for (rank = occupied_rank + 1, file = occupied_file - 1;
             rank <= 7 && file >= 0; rank++, file--) {
            attacks |= (1ULL << (rank * 8 + file));
            if ((1ULL << (rank * 8 + file)) & blockers)
                break;
        }
        // Top right
        for (rank = occupied_rank - 1, file = occupied_file + 1;
             rank >= 0 && file <= 7; rank--, file++) {
            attacks |= (1ULL << (rank * 8 + file));
            if ((1ULL << (rank * 8 + file)) & blockers)
                break;
        }
        return attacks;
    }

    // Rook attack table
    uint64_t mask_rook_attacks(int square) {
        // Attacks bitboard;
        uint64_t attacks = 0ULL;

        // Ranks and files for attacking
        int rank, file;
        int occupied_rank = square / 8, occupied_file = square % 8;

        // Right
        for (file = occupied_file + 1; file <= 6; file++) {
            attacks |= (1ULL << (occupied_rank * 8 + file));
        }
        // Left
        for (file = occupied_file - 1; file >= 1; file--) {
            attacks |= (1ULL << (occupied_rank * 8 + file));
        }
        // Up
        for (rank = occupied_rank - 1; rank >= 1; rank--) {
            attacks |= (1ULL << (rank * 8 + occupied_file));
        }
        // Down
        for (rank = occupied_rank + 1; rank <= 6; rank++) {
            attacks |= (1ULL << (rank * 8 + occupied_file));
        }
        return attacks;
    }

    uint64_t generate_rook_attacks(int square, uint64_t blockers) {
        // Attacks bitboard;
        uint64_t attacks = 0ULL;

        // Ranks and files for attacking
        int rank, file;
        int occupied_rank = square / 8, occupied_file = square % 8;

        // Right
        for (file = occupied_file + 1; file <= 7; file++) {
            attacks |= (1ULL << (occupied_rank * 8 + file));
            if ((1ULL << (occupied_rank * 8 + file)) & blockers)
                break;
        }

        // Left
        for (file = occupied_file - 1; file >= 0; file--) {
            attacks |= (1ULL << (occupied_rank * 8 + file));
            if ((1ULL << (occupied_rank * 8 + file)) & blockers)
                break;
        }

        // Up
        for (rank = occupied_rank - 1; rank >= 0; rank--) {
            attacks |= (1ULL << (rank * 8 + occupied_file));
            if ((1ULL << (rank * 8 + occupied_file)) & blockers)
                break;
        }
        // Down
        for (rank = occupied_rank + 1; rank <= 7; rank++) {
            attacks |= (1ULL << (rank * 8 + occupied_file));
            if ((1ULL << (rank * 8 + occupied_file)) & blockers)
                break;
        }
        return attacks;
    }

  public:
    CBoard() {
        // Setup initial postion of pieces
        initialize();
        // Initialize leaper attacks ie. pawns, knights
        init_leaper_attacks();

        uint64_t blockers = 0ULL;
        set_bit(blockers, d1);
        set_bit(blockers, d6);
        set_bit(blockers, g4);
        set_bit(blockers, c4);
        set_bit(blockers, b2);

        for (int i = 0; i < 64; i++)
            if (i != d1 && i != d6 && i != g4 && i != c4 && i != b2)
                print_bitboard(generate_rook_attacks(i, blockers));

        print_bitboard(blockers);
    }

    /*****************************************************************
     =================================================================
                                    Enums
     =================================================================
     *****************************************************************/

    // Enumerate pieces
    enum Board {
        emptyB,
        whiteB,
        blackB,
        whitePawnsB,
        whiteRooksB,
        whiteKnightsB,
        whiteBishopsB,
        whiteQueensB,
        whiteKingB,
        blackPawnsB,
        blackRooksB,
        blackKnightsB,
        blackBishopsB,
        blackQueensB,
        blackKingB,
        occupiedB
    };

    // Enumerate squares
    // clang-format off
    enum Square { a8, b8, c8, d8, e8, f8, g8, h8,
           a7, b7, c7, d7, e7, f7, g7, h7,
           a6, b6, c6, d6, e6, f6, g6, h6,
           a5, b5, c5, d5, e5, f5, g5, h5,
           a4, b4, c4, d4, e4, f4, g4, h4,
           a3, b3, c3, d3, e3, f3, g3, h3,
           a2, b2, c2, d2, e2, f2, g2, h2,
           a1, b1, c1, d1, e1, f1, g1, h1 };
    // clang-format on

    // Enumerate colors
    enum Color { white, black };

    // Enumerate attacks
    // clang-format off
    enum Attack {whitePawnsA, blackPawnsA, whiteBishopsA, blackBishopsA, rooksA, knightsA, queensA, kingA};
    // clang-format on

    /****************************************************************
     ================================================================
                                Board
     ================================================================
     ****************************************************************/

    // Returns the board for a certain piece type
    uint64_t getPieceSet(Board board) { return pieceBB[board]; }

    // Print the bitboad for a specific piece
    void print_bitboard(uint64_t board) {
        std::cout << "\n  ---------------------------------\n";

        for (int rank = 0; rank < 8; rank++) {
            for (int file = 0; file < 8; file++) {

                // Print files
                if (!file) {
                    std::cout << 8 - rank << " | ";
                }

                int square = rank * 8 + file;
                // Print 1 if piece is in location else 0
                if (get_bit(board, square) == 0)
                    std::cout << (get_bit(board, square)) << " | ";
                else
                    std::cout << "\033[1;33m" << (get_bit(board, square))
                              << "\033[0m | ";
            }
            std::cout << "\n  ---------------------------------\n";
        }

        // Print ranks
        std::cout << "    a   b   c   d   e   f   g   h";

        std::cout << "\n\n    Bitboard: " << board << "\n\n";
    }

    /****************************************************************
     ================================================================
                            Bit Manipulation
     ================================================================
     ****************************************************************/

    // Set initial position for all the pieces on the board
    void initialize() {
        // Pawns
        pieceBB[whitePawnsB] = 0x00FF000000000000ULL;
        pieceBB[blackPawnsB] = 0x000000000000FF00ULL;

        // Rooks
        pieceBB[whiteRooksB] = 0x8100000000000000ULL;
        pieceBB[blackRooksB] = 0x0000000000000081ULL;

        // Knigts
        pieceBB[whiteKnightsB] = 0x4200000000000000ULL;
        pieceBB[blackKnightsB] = 0x0000000000000042ULL;

        // // Bishops
        pieceBB[whiteBishopsB] = 0x2400000000000000ULL;
        pieceBB[blackBishopsB] = 0x0000000000000024ULL;

        // // Queens
        pieceBB[whiteQueensB] = 0x0800000000000000ULL;
        pieceBB[blackQueensB] = 0x0000000000000008ULL;

        // // King
        pieceBB[whiteKingB] = 0x1000000000000000ULL;
        pieceBB[blackKingB] = 0x0000000000000010ULL;

        // // Combined
        pieceBB[whiteB] = pieceBB[whitePawnsB] | pieceBB[whiteRooksB] |
                          pieceBB[whiteKnightsB] | pieceBB[whiteBishopsB] |
                          pieceBB[whiteQueensB] | pieceBB[whiteKingB];

        pieceBB[blackB] = pieceBB[blackPawnsB] | pieceBB[blackRooksB] |
                          pieceBB[blackKnightsB] | pieceBB[blackBishopsB] |
                          pieceBB[blackQueensB] | pieceBB[blackKingB];
        ;
        pieceBB[occupiedB] = pieceBB[whiteB] | pieceBB[blackB];
    }

    // Get bit at specific positon for a bitboard
    int get_bit(uint64_t bitboard, int square) {
        return (bitboard >> square) & 1ULL;
    }

    // Set a specific bit for a bitboard
    void set_bit(uint64_t &bitboard, int square) {
        bitboard |= (1ULL << square);
    }

    // Pop a specific bit for a bitboard
    void pop_bit(uint64_t &bitboard, int square) {
        bitboard &= ~(1ULL << square);
    }
};

#endif
