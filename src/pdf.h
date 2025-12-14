#ifndef PDF_H
#define PDF_H

#include "onb.h"
#include "collidable.h"

/**
 * A class for specifying pdfs and generating values from them
 */
class pdf {
    public:
        virtual ~pdf() {}

        /**
         * Returns the pdf distribution value in the given direction
         * @param direction direction of vector
         * @return pdf value associated with direction
         */
        virtual double value(const vec3& direction) const = 0;

        /**
         * Returns a direction weighted by this pdf
         * @return pdf weighted direction
         */
        virtual vec3 generate() const = 0;
};

/**
 * A uniform pdf over a unit sphere
 */
class sphere_pdf : public pdf {
    public:
        /**
         * Creates a uniform pdf generator over a unit sphere
         */
        sphere_pdf() {}

        double value(const vec3& direction) const override {
            return 1/ (4 * M_PI);
        }

        vec3 generate() const override {
            return randvec3().normalize();
        }
};

/**
 * A cosine-weighted pdf over a hemisphere
 */
class cosine_pdf : public pdf {
    public:
        /**
         * Creates a cosine-weighted pdf over a hemisphere in the given direction
         * @param k direction of hemisphere
         */
        cosine_pdf(const vec3& k) : ijk(k) {}

        double value(const vec3& direction) const override {
            double cosine_theta = vec3::dot(direction.normalize(), ijk.k());
            return std::fmax(0, cosine_theta/M_PI);
        }

        vec3 generate() const override {
            return ijk.transform(random_cosine_direction());
        }

    private:
        /**
         * The orthonormal basis vectors of the hemisphere
         */
        onb ijk;
};

/**
 * A pdf for any collidable object
 */
class collidable_pdf : public pdf {
    public:
        collidable_pdf(const collidable& objects, const vec3& origin)
        : objects(objects), origin(origin)
        {}

        double value(const vec3& direction) const override {
            return objects.pdf_value(origin, direction);
        }

        vec3 generate() const override {
            return objects.random(origin);
        }

    private:
        const collidable& objects;
        vec3 origin;
};

/**
 * A class for mixing pdfs
 */
class mixture_pdf : public pdf {
    public:
        /**
         * Creates a mixed pdf of two pdfs
         * @param p0 pdf to mix
         * @param p1 pdf to mix
         */
        mixture_pdf(shared_ptr<pdf> p0, shared_ptr<pdf> p1) {
            p[0] = p0;
            p[1] = p1;
        }

        double value(const vec3& direction) const override {
            return 0.5 * p[0]->value(direction) + 0.5 * p[1]->value(direction);
        }

        vec3 generate() const override {
            if (random_double() < 0.5)
                return p[0]->generate();
            else
                return p[1]->generate();
        }

    private:
        /**
         * The pdfs to mix
         */
        shared_ptr<pdf> p[2];
};
#endif