#define STENCIL_SZ 4  // I CHANGED THIS FROM 20

typedef struct {
  double x;
  //  double v[5];
} GridPt;

void apply_stencil_v1(const GridPt* grid_src, GridPt* grid_dst, int N);
void apply_stencil_v2(const GridPt* grid_src, GridPt* grid_dst, int N);
void apply_stencil_v3(const GridPt* grid_src, GridPt* grid_dst, int N);
