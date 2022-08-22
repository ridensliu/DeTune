/*
 *  Delaunay.cpp
 *
 *  Gilles Dumoulin's C++ implementation of Paul Bourke's Delaunay triangulation algorithm.
 *  Source: http://local.wasp.uwa.edu.au/~pbourke/papers/triangulate/cpp.zip
 *  Paper:  http://local.wasp.uwa.edu.au/~pbourke/papers/triangulate/index.html
 *
 */

#include "Delaunay.h"

using namespace std; 

////////////////////////////////////////////////////////////////////////
// CircumCircle() :
//   Return true if a point (xp,yp) is inside the circumcircle made up
//   of the points (x1,y1), (x2,y2), (x3,y3)
//   The circumcircle centre is returned in (xc,yc) and the radius r
//   Note : A point on the edge is inside the circumcircle

// 如果一个点(xp,yp)在由(x1,y1)、(x2,y2)、(x3,y3)组成的圆周内，返回真。
// 圆心在(xc,yc)中返回，半径为r。
// 注：边上的点认为在圆周内。
////////////////////////////////////////////////////////////////////////

int CircumCircle(double xp, double yp, double x1, double y1, double x2, 
double y2, double x3, double y3, double &xc, double &yc, double &r){
  double m1, m2, mx1, mx2, my1, my2;
  double dx, dy, rsqr, drsqr;

    // Check for coincident points
    // 检查重合的点
    if(abs(y1 - y2) < EPSILON && abs(y2 - y3) < EPSILON)
        return(false);
    
    if(abs(y2-y1) < EPSILON){
        m2 = - (x3 - x2) / (y3 - y2);
        mx2 = (x2 + x3) / 2.0;
        my2 = (y2 + y3) / 2.0;
        xc = (x2 + x1) / 2.0;
        yc = m2 * (xc - mx2) + my2;
    }else if(abs(y3 - y2) < EPSILON){
        m1 = - (x2 - x1) / (y2 - y1);
        mx1 = (x1 + x2) / 2.0;
        my1 = (y1 + y2) / 2.0;
        xc = (x3 + x2) / 2.0;
        yc = m1 * (xc - mx1) + my1;
    }else{
        m1 = - (x2 - x1) / (y2 - y1);
        m2 = - (x3 - x2) / (y3 - y2);
        mx1 = (x1 + x2) / 2.0;
        mx2 = (x2 + x3) / 2.0;
        my1 = (y1 + y2) / 2.0;
        my2 = (y2 + y3) / 2.0;
        xc = (m1 * mx1 - m2 * mx2 + my2 - my1) / (m1 - m2);
        yc = m1 * (xc - mx1) + my1;
    }
    
    dx = x2 - xc;
    dy = y2 - yc;
    rsqr = dx * dx + dy * dy;
    r = sqrt(rsqr);
    dx = xp - xc;
    dy = yp - yc;
    drsqr = dx * dx + dy * dy;
    
    return((drsqr <= rsqr) ? true : false);
}

///////////////////////////////////////////////////////////////////////////////
// Triangulate() :
//   Triangulation subroutine
//   Takes as input NV vertices in array pxyz
//   Returned is a list of ntri triangular faces in the array v
//   These triangles are arranged in a consistent clockwise order.
//   The triangle array 'v' should be malloced to 3 * nv
//   The vertex array pxyz must be big enough to hold 3 more points
//   The vertex array must be sorted in increasing x values say
//
//   qsort(p,nv,sizeof(XYZ),XYZCompare);

// 三角测量子程序
// 接受数组 pxyz 中的 NV 顶点作为输入。
// 返回的是数组 v 中的 ntri 三角形面的列表。
// 这些三角形以一致的顺时针顺序排列。
// 三角形数组'v'应该被malloc为3 * nv     // malloc: 动态分配内存空间
// 顶点数组pxyz必须足够大，以便再容纳3个点
// 顶点数组必须按x值递增进行排序，例如
// qsort(p,nv,sizeof(XYZ),XYZCompare);
///////////////////////////////////////////////////////////////////////////////

int Triangulate(int nv, XYZ pxyz[], ITRIANGLE v[], int &ntri)
{
    int *complete = NULL;
    IEDGE *edges = NULL;
    IEDGE *p_EdgeTemp;
    int nedge = 0;
    int trimax, emax = 200;
    int status = 0;
    
    int inside;
    int i, j, k;
    double xp, yp, x1, y1, x2, y2, x3, y3, xc, yc, r;
    double xmin, xmax, ymin, ymax, xmid, ymid;
    double dx, dy, dmax;
    
    // Allocate memory for the completeness list, flag for each triangle
    // 为完整性列表分配内存，为每个三角形分配flag。
    trimax = 4 * nv;
    complete = new int[trimax];
    // Allocate memory for the edge list
    // 为边列表分配内存
    edges = new IEDGE[emax];
    
    // Find the maximum and minimum vertex bounds.
    // This is to allow calculation of the bounding triangle
    // 找到最大和最小的顶点边界。
    // 这是为了能够计算出边界三角形的情况
    xmin = pxyz[0].x;
    ymin = pxyz[0].y;
    xmax = xmin;
    ymax = ymin;
    for(i = 1; i < nv; i++){
        if (pxyz[i].x < xmin) xmin = pxyz[i].x;
        if (pxyz[i].x > xmax) xmax = pxyz[i].x;
        if (pxyz[i].y < ymin) ymin = pxyz[i].y;
        if (pxyz[i].y > ymax) ymax = pxyz[i].y;
    }
    dx = xmax - xmin;
    dy = ymax - ymin;
    dmax = (dx > dy) ? dx : dy;
    xmid = (xmax + xmin) / 2.0;
    ymid = (ymax + ymin) / 2.0;
    
   // Set up the supertriangle
   // This is a triangle which encompasses all the sample points.
   // The supertriangle coordinates are added to the end of the
   // vertex list. The supertriangle is the first triangle in
   // the triangle list.
    // 设置超三角形
    // 这是一个包含了所有样本点的三角形。
    // 超三角形的坐标被添加到顶点列表的末尾。超级三角形是三角形列表中的第一个三角形。
    pxyz[nv+0].x = xmid - 20 * dmax;
    pxyz[nv+0].y = ymid - dmax;
    pxyz[nv+0].z = 0.0;
    pxyz[nv+1].x = xmid;
    pxyz[nv+1].y = ymid + 20 * dmax;
    pxyz[nv+1].z = 0.0;
    pxyz[nv+2].x = xmid + 20 * dmax;
    pxyz[nv+2].y = ymid - dmax;
    pxyz[nv+0].z = 0.0;
    v[0].p1 = nv;
    v[0].p2 = nv+1;
    v[0].p3 = nv+2;
    complete[0] = false;
    ntri = 1;
    
    
    // Include each point one at a time into the existing mesh
    // 将每个点逐一纳入现有的mesh中。
    for(i = 0; i < nv; i++){
        xp = pxyz[i].x;
        yp = pxyz[i].y;
        nedge = 0;
        
        // Set up the edge buffer.
        // If the point (xp,yp) lies inside the circumcircle then the
        // three edges of that triangle are added to the edge buffer
        // and that triangle is removed.
        // 设置edge buffer。
        // 如果点（xp,yp）位于圆周内，那么
        // 该三角形的三条边被添加到edge buffer中。
        // 而该三角形则被删除。
        for(j = 0; j < ntri; j++){
            if(complete[j])
                continue;
            x1 = pxyz[v[j].p1].x;
            y1 = pxyz[v[j].p1].y;
            x2 = pxyz[v[j].p2].x;
            y2 = pxyz[v[j].p2].y;
            x3 = pxyz[v[j].p3].x;
            y3 = pxyz[v[j].p3].y;
            inside = CircumCircle(xp, yp, x1, y1, x2, y2, x3, y3, xc, yc, r);
            //    if (xc + r < xp)
            
            // Suggested
            // if (xc + r + EPSILON < xp)
            if (xc < xp && ((xp-xc)*(xp-xc)) > r)
                complete[j] = true;
            
            if(inside){
                // Check that we haven't exceeded the edge list size
                if(nedge + 3 >= emax){
                    emax += 100;
                    p_EdgeTemp = new IEDGE[emax];
                    for (int i = 0; i < nedge; i++) { // Fix by John Bowman
                        p_EdgeTemp[i] = edges[i];
                    }
                    delete []edges;
                    edges = p_EdgeTemp;
                }
                
                edges[nedge+0].p1 = v[j].p1;
                edges[nedge+0].p2 = v[j].p2;
                edges[nedge+1].p1 = v[j].p2;
                edges[nedge+1].p2 = v[j].p3;
                edges[nedge+2].p1 = v[j].p3;
                edges[nedge+2].p2 = v[j].p1;
                nedge += 3;
                v[j] = v[ntri-1];
                complete[j] = complete[ntri-1];
                ntri--;
                j--;
            }
        }
        
        // Tag multiple edges
        // Note: if all triangles are specified anticlockwise then all
        // interior edges are opposite pointing in direction.
        // 标记多条边
        // 注意：如果所有三角形都被指定为逆时针，那么所有内部边缘的方向都是相反的。
        for(j = 0; j < nedge - 1; j++){
            for(k = j + 1; k < nedge; k++){
                if((edges[j].p1 == edges[k].p2) && (edges[j].p2 == edges[k].p1)){
                    edges[j].p1 = -1;
                    edges[j].p2 = -1;
                    edges[k].p1 = -1;
                    edges[k].p2 = -1;
                }
                // Shouldn't need the following, see note above */
                if((edges[j].p1 == edges[k].p1) && (edges[j].p2 == edges[k].p2)){
                    edges[j].p1 = -1;
                    edges[j].p2 = -1;
                    edges[k].p1 = -1;
                    edges[k].p2 = -1;
                }
            }
        }

        // Form new triangles for the current point
        // Skipping over any tagged edges.
        // All edges are arranged in clockwise order.
        // 为当前点形成新的三角形，跳过任何标记的边。
        // 所有边都按顺时针顺序排列。
        for(j = 0; j < nedge; j++) {
            if(edges[j].p1 < 0 || edges[j].p2 < 0)
                continue;

            if (ntri >= trimax) {
                break;
            }
            
            v[ntri].p1 = edges[j].p1;
            v[ntri].p2 = edges[j].p2;
            v[ntri].p3 = i;
            complete[ntri] = false;
            ntri++;
        }
        
        if (ntri >= trimax) {
            break;
        }
    }
    
    // Remove triangles with supertriangle vertices
    // These are triangles which have a vertex number greater than nv
    // 删除具有超三角形顶点的三角形
    // 这些是顶点数量大于nv的三角形。
    for(i = 0; i < ntri; i++) {
        if(v[i].p1 >= nv || v[i].p2 >= nv || v[i].p3 >= nv) {
            v[i] = v[ntri-1];
            ntri--;
            i--;
        }
    }
    
    delete[] edges;
    delete[] complete;
    return 0;
}

int XYZCompare(const void *v1, const void *v2){
  XYZ *p1, *p2;
    
  p1 = (XYZ*)v1;
  p2 = (XYZ*)v2;
  if(p1->x < p2->x)
    return(-1);
  else if(p1->x > p2->x)
         return(1);
       else
         return(0);
}
