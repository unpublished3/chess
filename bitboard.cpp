#include <cstdint>

class CBoard {
    // Array of pieces defined in the order of the enum
    uint64_t pieceBB[14];
    uint64_t emptyBB;
    uint64_t occupiedBB;

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
};