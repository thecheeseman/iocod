#ifndef QMATH_H
#define QMATH_H

#include <math.h>

#include "types/vector.h"

#define dot_product(x,y)		((x)[0]*(y)[0]+(x)[1]*(y)[1]+(x)[2]*(y)[2])
#define vector_subtract(a,b,c)	((c)[0]=(a)[0]-(b)[0],(c)[1]=(a)[1]-(b)[1],(c)[2]=(a)[2]-(b)[2])
#define vector_add(a,b,c)		((c)[0]=(a)[0]+(b)[0],(c)[1]=(a)[1]+(b)[1],(c)[2]=(a)[2]+(b)[2])
#define vector_copy(a,b)		((b)[0]=(a)[0],(b)[1]=(a)[1],(b)[2]=(a)[2])
#define	vector_scale(v, s, o)	((o)[0]=(v)[0]*(s),(o)[1]=(v)[1]*(s),(o)[2]=(v)[2]*(s))
#define	vector_ma(v, s, b, o)	((o)[0]=(v)[0]+(b)[0]*(s),(o)[1]=(v)[1]+(b)[1]*(s),(o)[2]=(v)[2]+(b)[2]*(s))
#define vector_clear(a)			((a)[0]=(a)[1]=(a)[2]=0)
#define vector_negate(a,b)		((b)[0]=-(a)[0],(b)[1]=-(a)[1],(b)[2]=-(a)[2])
#define vector_set(v, x, y, z)	((v)[0]=(x), (v)[1]=(y), (v)[2]=(z))
#define vector4_copy(a,b)		((b)[0]=(a)[0],(b)[1]=(a)[1],(b)[2]=(a)[2],(b)[3]=(a)[3])

#define	snap_vector(v) {v[0]=((int)(v[0]));v[1]=((int)(v[1]));v[2]=((int)(v[2]));}

extern vec3_t vec3_origin;

void clear_bounds(vec3_t mins, vec3_t maxs);
void add_point_to_bounds(const vec3_t v, vec3_t mins, vec3_t maxs);
void cross_product(const vec3_t v1, const vec3_t v2, vec3_t cross);
vec_t vector_normalize(vec3_t v);
vec_t vector_normalize2(const vec3_t v, vec3_t out);
vec_t vector_length(const vec3_t v);
vec_t vector_length_squared(const vec3_t v);
vec_t distance(const vec3_t p1, const vec3_t p2);
vec_t distance_squared(const vec3_t p1, const vec3_t p2);

#endif /* QMATH_H */
