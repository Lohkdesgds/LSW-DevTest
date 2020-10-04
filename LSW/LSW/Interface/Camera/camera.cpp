#include "camera.h"

namespace LSW {
	namespace v5 {
		namespace Interface {




			Camera::Camera(const Camera& c)
			{
				t = c.t;
			}

			Camera& Camera::operator=(const Camera& c)
			{
				t = c.t;
				return *this;
			}

			void Camera::invert()
			{
				al_invert_transform(&t);
			}

			bool Camera::check_inverse(const float tol)
			{
				return al_check_inverse(&t, tol);
			}

			void Camera::identity()
			{
				al_identity_transform(&t);
			}

			void Camera::build_transform(const float x, const float y, const float sx, const float sy, const float th)
			{
				al_build_transform(&t, x, y, sx, sy, th);
			}

			void Camera::build_3d_transform(const float x, const float y, const float z, const float lx, const float ly, const float lz, const float ux, const float uy, const float uz)
			{
				al_build_camera_transform(&t, x, y, z, lx, ly, lz, ux, uy, uz);
			}

			bool Camera::classic_transform(std::shared_ptr<Bitmap> bmp, const float x, const float y, const float sx, const float sy, const float r)
			{
				last_targ = bmp;
				return classic_transform(x, y, sx, sy, r);
			}

			bool Camera::classic_transform(const float x, const float y, const float sx, const float sy, const float r)
			{
				cl_2d.x = x;
				cl_2d.y = y;
				cl_2d.sx = sx;
				cl_2d.sy = sy;
				cl_2d.rot = r;

				return classic_refresh();
			}

			bool Camera::classic_refresh()
			{
				if (!last_targ) return false;

				const int dx = last_targ->get_width();
				const int dy = last_targ->get_height();

				identity();
				rotate(cl_2d.rot);
				scale(0.5f * dx * cl_2d.sx, 0.5f * dy * cl_2d.sy);
				translate(0.5f * dx - cl_2d.x * dx * 0.5f, 0.5f * dy + cl_2d.y * dy * 0.5f); // classic cartesian +

				return true;
			}

			bool Camera::classic_update(std::shared_ptr<Bitmap> bmp)
			{
				last_targ = bmp;
				return classic_refresh();
			}

			classic_2d& Camera::get_classic()
			{
				return cl_2d;
			}

			void Camera::rotate(const float r)
			{
				al_rotate_transform(&t, r);
			}

			void Camera::scale(const float sx, const float sy)
			{
				al_scale_transform(&t, sx, sy);
			}

			void Camera::transform(float& x, float& y)
			{
				al_transform_coordinates(&t, &x, &y);
			}

			void Camera::transform_3d(float& x, float& y, float& z)
			{
				al_transform_coordinates_3d(&t, &x, &y, &z);
			}

			void Camera::compose(const Camera& c)
			{
				al_compose_transform(&t, &c.t);
			}

			void Camera::translate(const float x, const float y)
			{
				al_translate_transform(&t, x, y);
			}

			void Camera::translate_3d(const float x, const float y, const float z)
			{
				al_translate_transform_3d(&t, x, y, z);
			}

			void Camera::apply()
			{
				al_use_transform(&t);
			}

		}
	}
}