#pragma once

#include <memory>
#include <functional>

#include "..\..\Handling\Abort\abort.h"
#include "..\..\Handling\Initialize\initialize.h"

#include "..\Bitmap\bitmap.h"

namespace LSW {
	namespace v5 {
		namespace Interface {

			struct classic_2d {
				float x = 0,
					y = 0,
					sx = 1,
					sy = 1,
					rot = 0;
			};

			class Camera {
				ALLEGRO_TRANSFORM t{};
				classic_2d cl_2d;
				std::shared_ptr<Bitmap> last_targ;
			public:
				Camera() = default;
				Camera(const Camera&);

				Camera& operator=(const Camera&);

				// invert matrix
				void invert();
				// float = tolerance. true if invertible
				bool check_inverse(const float);
				// set matrix as identity
				void identity();

				// builds based on X, Y, ScaleX, ScaleY and Theta.
				void build_transform(const float, const float, const float, const float, const float);
				// builds on POS X,Y,Z, LOOKING X,Y,Z, and considering UP on X,Y,Z
				void build_3d_transform(const float, const float, const float, const float, const float, const float, const float = 0.0f, const float = 1.0f, const float = 0.0f);

				// classic transform based on bitmap. X, Y, SX, SY, ROT
				bool classic_transform(std::shared_ptr<Bitmap>, const float, const float, const float, const float, const float);
				// classic transform (assuming there is a bitmap set). X, Y, SX, SY, ROT
				bool classic_transform(const float, const float, const float, const float, const float);
				// reset latest classic transform
				bool classic_refresh();
				// update bitmap reference for classic refresh
				bool classic_update(std::shared_ptr<Bitmap>);

				// get reference to classic data (saved via classic_ commands)
				const classic_2d& get_classic() const;
				// get reference to classic data (saved via classic_ commands)
				classic_2d& get_classic();

				// rotate the matrix
				void rotate(const float);
				// scale the matrix
				void scale(const float, const float);

				// transform coordinates using this matrix
				void transform(float&, float&);
				// transform coordinates using this matrix
				void transform_3d(float&, float&, float&);

				// compose (combine) two Cameras by matrix multiplication
				void compose(const Camera&);

				// translate matrix
				void translate(const float, const float);
				// translate matrix 3d
				void translate_3d(const float, const float, const float);

				// apply transform to current thread display (const because it doesn't change itself)
				void apply() const;
			};

		}
	}
}