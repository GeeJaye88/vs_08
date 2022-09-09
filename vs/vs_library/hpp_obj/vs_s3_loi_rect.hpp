////////////////////////////////////////////////////////////////////////////////

// ---------- vs_s3_loi_rect.hpp ----------
/*!
\file vs_s3_loi_rect.hpp
\brief
\author Gareth Edwards
*/


#pragma once

struct Point
{
	float X = 0;
	float Y = 0;
};

struct Rect
{
	float X = 0;
	float Y = 0;
	float Width = 0;
	float Height = 0;
};

bool SegmentIntersectRectangle(
        double rectangleMinX,
        double rectangleMinY,
        double rectangleMaxX,
        double rectangleMaxY,
        double p1X,
        double p1Y,
        double p2X,
        double p2Y
	)
    {

        // ---- find min and max X for the segment
			double minX = p1X;
			double maxX = p2X;
			if (p1X > p2X)
			{
				minX = p2X;
				maxX = p1X;
			}

        // ---- find the intersection of the segment's and rectangle's x-projections
			if (maxX > rectangleMaxX)
			{
				maxX = rectangleMaxX;
			}
			if (minX < rectangleMinX)
			{
				minX = rectangleMinX;
			}
			if (minX > maxX) // If their projections do not intersect return false
			{
				return false;
			}

        // ---- find corresponding min and max Y for min and max X we found before
			double minY = p1Y;
			double maxY = p2Y;

			double dx = p2X - p1X;

			if ( abs(dx) > 0.0000001 )
			{
				double a = (p2Y - p1Y)/dx;
				double b = p1Y - a*p1X;
				minY = a*minX + b;
				maxY = a*maxX + b;
			}

			if ( minY > maxY )
			{
				double tmp = maxY;
				maxY = minY;
				minY = tmp;
			}

        // ---- find the intersection of the segment's and rectangle's y-projections
			if (maxY > rectangleMaxY)
			{
				maxY = rectangleMaxY;
			}

			if (minY < rectangleMinY)
			{
				minY = rectangleMinY;
			}

			if (minY > maxY) // If Y-projections do not intersect return false
			{
				return false;
			}

        return true;
    }


bool LineIntersectsRect(Point p1, Point p2, Rect r)
	{
		return SegmentIntersectRectangle(r.X, r.Y, r.X + r.Width, r.Y + r.Height, p1.X, p1.Y, p2.X, p2.Y);
	}


bool line_intersect(
		float x1,
		float y1,
		float x2,
		float y2,
		float x3,
		float y3,
		float x4,
		float y4,
		float *x,
		float *y
	)
{

	// ---- check if none of the lines are of length 0
		if ( (x1 == x2 && y1 == y2) || (x3 == x4 && y3 == y4)) {
			return false;
		}
		float denominator = ((y4 - y3) * (x2 - x1) - (x4 - x3) * (y2 - y1));

	// ---- lines are parallel
		if ( denominator == 0 ) {
			return false;
		}
		float ua = ((x4 - x3) * (y1 - y3) - (y4 - y3) * (x1 - x3)); // denominator
		float ub = ((x2 - x1) * (y1 - y3) - (y2 - y1) * (x1 - x3)); // denominator

	// ---- is the intersection along the segments
		if ( ( ua < 0 || ua > 1 ) || ( ub < 0 || ub > 1 ) ) {
			return false;
		}

	// ---- return the x and y coordinates of the intersection
		*x = x1 + ua * (x2 - x1);
		*y = y1 + ua * (y2 - y1);

	return true;
}


inline double Det(double a, double b, double c, double d)
{
	return a*d - b*c;
}


///Calculate intersection of two lines.
///\return true if found, false if not found or error
bool LineLineIntersect(
	double x1, double y1, //Line 1 start
	double x2, double y2, //Line 1 end
	double x3, double y3, //Line 2 start
	double x4, double y4, //Line 2 end
	double &ixOut, double &iyOut) //Output 
{
	//http://mathworld.wolfram.com/Line-LineIntersection.html

	double detL1 = Det(x1, y1, x2, y2);
	double detL2 = Det(x3, y3, x4, y4);
	double x1mx2 = x1 - x2;
	double x3mx4 = x3 - x4;
	double y1my2 = y1 - y2;
	double y3my4 = y3 - y4;

	double xnom = Det(detL1, x1mx2, detL2, x3mx4);
	double ynom = Det(detL1, y1my2, detL2, y3my4);
	double denom = Det(x1mx2, y1my2, x3mx4, y3my4);
	if(denom == 0.0)//Lines don't seem to cross
	{
		ixOut = NAN;
		iyOut = NAN;
		return false;
	}

	ixOut = xnom / denom;	
	iyOut = ynom / denom;
	if(!isfinite(ixOut) || !isfinite(iyOut)) //Probably a numerical issue
		return false;

	return true; //All OK
}


bool isBetween(float ax, float ay, float bx, float by, float cx, float cy)
{
	float crossproduct = (cy - ay) * (bx - ax) - (cx - ax) * (by - ay);

	float epsilon = 0.001f;

	// compare versus epsilon for floating point values, or != 0 if using integers
	if ( abs(crossproduct) > epsilon) return false;

	float dotproduct = (cx - ax) * (bx - ax) + (cy - ay)*(by - ay);
	if (dotproduct < 0) return false;

	float squaredlengthba = (bx - ax)*(bx - ax) + (by - ay)*(by - ay);
	if (dotproduct > squaredlengthba) return false;


	return true;
}


bool PointOnLine2D (float ax, float ay, float bx, float by, float cx, float cy)
{

	float epsilon = 0.001f;

    // ensure points are collinear
    float zero = (bx - ax) * (cy - ay) - (cx - ax) * (by - ay);
    if (zero > epsilon || zero < -epsilon) return false;

    // check if x-coordinates are not equal
    if (ax - bx > epsilon || bx - ax > epsilon)
        // ensure x is between a.x & b.x (use tolerance)
        return ax > bx
            ? cx + epsilon > bx && cx - epsilon < ax
            : cx + epsilon > ax && cx - epsilon < bx;

    // ensure y is between a.y & b.y (use tolerance)
    return ay > by
        ? cy + epsilon > by && cy - epsilon < ay
        : cy + epsilon > ay && cy - epsilon < by;
}


// ---------- Callback_S3_Loi_Blade_Configure ----------
INT ObjConfig::Callback_S3_Loi_Blade_Configure(vs_system::ElmContext *context)
{

	// ---- begin configuration profiling
		context->BeginConfig();


	// ---- local
		FLOAT *sd = context->GetGfxFloatParameter();
		FLOAT *pd = context->GetFloatParameter();


	// ---- get supplied and parameter data
		UINT type   = (UINT)pd[BOX_FACE_TYPE];
		UINT ap_ref = (UINT)pd[BOX_FACE_AP_REF];


	// ---- atlas page
		context->SetAtlasPage(0, ap_ref);


	// ---- overide ?
		UINT format = (UINT)(VS_XYZ | VS_NORMAL); // wireframe
		if ( !context->GetOveride() )
		{
			context->SetAlphaBlending(FALSE);
			context->SetColourVertex(FALSE);
			context->SetMultiState(FALSE);

			switch ( type )
			{
				default:
				case 0: // wireframe
					format = (UINT)(VS_XYZ | VS_DIFFUSE);
					context->SetColourVertex(TRUE);
					context->SetSolid(FALSE);
					context->SetTextured(FALSE);
					break;
				case 1: // solid
					format = (UINT)(VS_XYZ | VS_DIFFUSE);
					context->SetAlphaBlending(TRUE);
					context->SetColourVertex(TRUE);
					context->SetSolid(TRUE);
					context->SetTextured(FALSE);
					break;
				case 2: // textured
					format = (UINT)(VS_XYZ | VS_NORMAL | VS_TEX1);
					context->SetSolid(TRUE);
					context->SetTextured(TRUE);
					break;
			}

			context->SetVisible(true);
		}


	// ---- index buffer
		context->SetIndexCount(0);
		context->SetIndexBufferCallback(NULL);


	// ---- primitive type
		context->SetPrimitiveType(VS_TRIANGLELIST);


	// ---- vertex buffer ( 2 * sides * 2 triangles * 3 vertices == 12 )
		context->SetVertexCount(12);
		context->SetVertexFormat(format);
		context->SetVertexSize(context->GetVertexFormat());
		context->SetVertexBufferCallback(&Callback_S3_Loi_Blade_GetVertexBuffer);


	// ---- primitive parameters ( 2 * sides * 2 triangles == 4 )
		context->SetPrimitiveBytes(
				context->GetVertexSize() *
				context->GetVertexCount()  *
				sizeof(float)
			);
		context->SetPrimitiveCount(4);


	// ---- end configuration profiling
		context->EndConfig();

	return VS_OK;
}


// ---------- Callback_S3_Loi_Blade_GetVertexBuffer ----------
INT ObjConfig::Callback_S3_Loi_Blade_GetVertexBuffer(vs_system::ElmContext *context)
{

	// ---- local
		FLOAT *sd = context->GetGfxFloatParameter();
		FLOAT *pd = context->GetFloatParameter();


	// ---- get supplied and parameter data
		UINT  mode    = (UINT)pd[BOX_FACE_MODE];
		UINT  type    = (UINT)pd[BOX_FACE_TYPE];
		UINT  ap_ref  = (UINT)pd[BOX_FACE_AP_REF];
		UINT  ap_sect = (UINT)pd[BOX_FACE_AP_SECT];
		FLOAT red     = pd[BOX_FACE_RED];
		FLOAT green   = pd[BOX_FACE_GREEN];
		FLOAT blue    = pd[BOX_FACE_BLUE];

	// ---- uncalculated
		FLOAT width = pd[vs_system::ObjConfig::BOX_FACE_WIDTH];
		FLOAT depth = pd[vs_system::ObjConfig::BOX_FACE_DEPTH];

	// ---- location
		FLOAT x_off = pd[vs_system::ObjConfig::BOX_FACE_X];
		FLOAT y_off = pd[vs_system::ObjConfig::BOX_FACE_Y];
		FLOAT z_off = pd[vs_system::ObjConfig::BOX_FACE_Z];

	// ---- other parameter data
		FLOAT angle = pd[vs_system::ObjConfig::BOX_FLAG_1];
		FLOAT y_top = pd[vs_system::ObjConfig::BOX_FLAG_2];
		FLOAT y_bot = pd[vs_system::ObjConfig::BOX_FLAG_3];

		FLOAT x3 = pd[vs_system::ObjConfig::BOX_FLAG_4];
		FLOAT y3 = pd[vs_system::ObjConfig::BOX_FLAG_5];
		FLOAT x4 = pd[vs_system::ObjConfig::BOX_FLAG_6];
		FLOAT y4 = pd[vs_system::ObjConfig::BOX_FLAG_7];


	// ---- vertex
		UINT vertex_size = context->GetVertexSize();
		UINT vertex_count = context->GetVertexCount();
		UINT vertex_format = context->GetVertexFormat();
		FLOAT *f = context->GetVertexBufferPtr();


	// ---- diffuse
		union colour_union_t {
			DWORD d;
			FLOAT f;
		} white;
		white.d = VS_D3DCOLOR_COLORVALUE(
			red,
			green,
			blue,
			0.8f
		);


	// ---- complete vertex as format 
		struct vtx
		{
			float x, y, z, d, xn, yn, zn, u, v;
		};


	// ---- proportional to depth
		FLOAT aw = depth * 0.025f;


	#define NEW_LLI_CODE

	#if defined (NEW_LLI_CODE)

		VsVector2 r_xy[2]
		{
			{ x3, y3 },
			{ x4, y4 }
		};

	#else

	// ---- calculate the two intersects
		float rmin_x = -width / 2;
		float rmin_y = -depth / 2;
		float rmax_x = width / 2;
		float rmax_y = depth / 2;


	// ---- calculate two rect edge intersects

		// 2D point
		struct punt { float x; float y; };

		punt s_xy[5] = // outline of reect
		{
			{ rmin_x, rmin_y },
			{ rmin_x, rmax_y },
			{ rmax_x, rmax_y },
			{ rmax_x, rmin_y },
			{ rmin_x, rmin_y }
		};

		punt p_xy[2] = // zero angle line 
		{
			{ -900, 0 },
			{  900, 0 },
		};

		UINT r_index = 0;
		punt r_xy[2] = // intersect results
		{
			{ 0, 0 },
			{ 0, 0 },
		};

		// ---- edges
		FLOAT ar = angle * (FLOAT)VS_RADIANS_PER_DEGREE;
		double ix=0, iy=0;
		for (UINT e = 0; e < 4; e++)
		{ 

			// edge line
			float x1 = s_xy[e].x;
			float y1 = s_xy[e].y;
			float x2 = s_xy[e+1].x;
			float y2 = s_xy[e+1].y;

			// blade line
			for (UINT l = 0; l < 1; l++)
			{  

				// first point
				float x3 = x_off;
				float y3 = z_off;

				// 2nd point 
				// x' = y*sin q + x*cos q
				// y' = y*cos q - x*sin q
				float x4 = p_xy[l].y * sin(ar) + p_xy[l].x * cos(ar) + x_off;
				float y4 = p_xy[l].y * cos(ar) - p_xy[l].x * sin(ar) + z_off;

				// calc intersect
				bool lli = LineLineIntersect(
						(double)x1, (double)y1, (double)x2, (double)y2,
						(double)x3, (double)y3, (double)x4, (double)y4,
						ix,
						iy
					);

				// chuck "out of bounds" results
				if ( lli )
				{
					if ( ( ix < rmin_x || ix > rmax_x ) || ( iy < rmin_y || iy > rmax_y ) )
						lli = false;
				}

				// result
				if ( lli )
				{
					if ( r_index < 2 )
					{
						r_xy[r_index].x = (float)ix;
						r_xy[r_index].y = (float)iy;
						r_index++;
					}

				}
			}
		}

		#endif

	// ---- front (fn) & back (bn) surface normals
		FLOAT vl = ap_sect == 1 ? 0.5f : 0;
		FLOAT vh = ap_sect == 1 ? 1 : 0.5f;
		VsVector3 fn = { 0, 0, 0 };
		VsVector3 bn = { 0, 0, 0 };
		{
			VsVector3 a = { 0, (y_top - y_bot), 0 };
			VsVector3 b = { r_xy[1].x - r_xy[0].x, 0, r_xy[1].y - r_xy[0].y };
			a.Normalise();
			b.Normalise();
			fn.CrossProduct(&a, &b);
			bn.x = -fn.x; bn.y = fn.y; bn.z = -fn.z;
		}


	// ---- y sample offset to "see" surface intersect, proportional to surface size
		FLOAT y_offset = 0.015f * aw;


	// ---- nudge y_top
		y_top += y_offset;


	// ---- blade vtx table
		//
		//  1 > 2
		//  ^ / v
		//  0 < 3
		//
		vtx blade_located[12] =
		{
			// x          y       z           xn     yn  zn     u   v
			{  r_xy[0].x, y_bot,  r_xy[0].y,  fn.x,  0,  fn.z,  0,  vl },
			{  r_xy[0].x, y_top,  r_xy[0].y,  fn.x,  0,  fn.z,  0,  vh },
			{  r_xy[1].x, y_top,  r_xy[1].y,  fn.x,  0,  fn.z,  0,  vh },

			{  r_xy[1].x, y_top,  r_xy[1].y,  fn.x,  0,  fn.z,  0,  vh },
			{  r_xy[1].x, y_bot,  r_xy[1].y,  fn.x,  0,  fn.z,  0,  vl },
			{  r_xy[0].x, y_bot,  r_xy[0].y,  fn.x,  0,  fn.z , 0,  vl },

			{  r_xy[1].x, y_bot,  r_xy[1].y,  bn.x,  0,  bn.z , 0,  vl },
			{  r_xy[1].x, y_top,  r_xy[1].y,  bn.x,  0,  bn.z,  0,  vh },
			{  r_xy[0].x, y_top,  r_xy[0].y,  bn.x,  0,  bn.z,  0,  vh },

			{  r_xy[0].x, y_top,  r_xy[0].y,  bn.x,  0,  bn.z,  0,  vh },
			{  r_xy[0].x, y_bot,  r_xy[0].y,  bn.x,  0,  bn.z,  0,  vl },
			{  r_xy[1].x, y_bot,  r_xy[1].y,  bn.x,  0,  bn.z , 0,  vl },
		};


	// ---- add vertex as per format from vtx table
		for (UINT vt = 0; vt < 12; vt++)
		{

				vtx *h = &blade_located[vt];

				if ( vertex_format & VS_XYZ )
				{
					*(f + 0) = h->x;
					*(f + 1) = h->y;
					*(f + 2) = h->z;
					f += 3;
				}

				if ( vertex_format & VS_NORMAL )
				{
					*(f + 0) = h->xn;
					*(f + 1) = h->yn;
					*(f + 2) = h->zn;
					f += 3;
				}

				if ( vertex_format & VS_DIFFUSE )
				{
					*(f) = white.f; f++;
				}

				if ( vertex_format & VS_SPECULAR )
				{
					*(f) = 1;
					f++;
				}

				if ( vertex_format & VS_TEX1 )
				{
					*(f + 0) = h->u;
					*(f + 1) = h->v;
					f+=2;
				}

		}

	return VS_OK;
}

