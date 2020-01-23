namespace ObjectBuffers {


// Front Verticies
#define VERTEX_FTR  0.5f,  0.5f,  0.5f
#define VERTEX_FTL -0.5f,  0.5f,  0.5f
#define VERTEX_FBL -0.5f, -0.5f,  0.5f
#define VERTEX_FBR  0.5f, -0.5f,  0.5f

// Back Verticies
#define VERTEX_BTR  0.5f,  0.5f, -0.5f
#define VERTEX_BTL -0.5f,  0.5f, -0.5f
#define VERTEX_BBL -0.5f, -0.5f, -0.5f
#define VERTEX_BBR  0.5f, -0.5f, -0.5f

// Create a colored cube
const float CubeBuffer[] = {
  // Face 1 (Front)
    VERTEX_FTR, VERTEX_FTL, VERTEX_FBL,
    VERTEX_FBL, VERTEX_FBR, VERTEX_FTR,
  // Face 2 (Back)
    VERTEX_BBR, VERTEX_BTL, VERTEX_BTR,
    VERTEX_BTL, VERTEX_BBR, VERTEX_BBL,
  // Face 3 (Top)
    VERTEX_FTR, VERTEX_BTR, VERTEX_BTL,
    VERTEX_BTL, VERTEX_FTL, VERTEX_FTR,
  // Face 4 (Bottom)
    VERTEX_FBR, VERTEX_FBL, VERTEX_BBL,
    VERTEX_BBL, VERTEX_BBR, VERTEX_FBR,
  // Face 5 (Left)
    VERTEX_FBL, VERTEX_FTL, VERTEX_BTL,
    VERTEX_FBL, VERTEX_BTL, VERTEX_BBL,
  // Face 6 (Right)
    VERTEX_FTR, VERTEX_FBR, VERTEX_BBR,
    VERTEX_BBR, VERTEX_BTR, VERTEX_FTR
};

#undef VERTEX_BBR
#undef VERTEX_BBL
#undef VERTEX_BTL
#undef VERTEX_BTR

#undef VERTEX_FBR
#undef VERTEX_FBL
#undef VERTEX_FTL
#undef VERTEX_FTR

}
