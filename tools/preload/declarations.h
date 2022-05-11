#ifdef DECLARATIONS_INCLUDE
#define DECLARE_1(n, t, p) \
        typedef t (*n##_t)(p); \
        extern n##_t n; 
#define DECLARE_2(n, t, p, p2) \
        typedef t (*n##_t)(p, p2); \
        extern n##_t n; 
#define DECLARE_3(n, t, p, p2, p3) \
        typedef t (*n##_t)(p, p2, p3); \
        extern n##_t n; 
#define DECLARE_4(n, t, p, p2, p3, p4) \
        typedef t (*n##_t)(p, p2, p3, p4); \
        extern n##_t n; 
#define DECLARE_7(n, t, p, p2, p3, p4, p5, p6, p7) \
        typedef t (*n##_t)(p, p2, p3, p4, p5, p6, p7); \
        extern n##_t n; 

#undef DECLARATIONS_INCLUDE
#endif

#ifdef DECLARATIONS_CLEANUP
#undef DECLARE_1
#undef DECLARE_2
#undef DECLARE_3
#undef DECLARE_4
#undef DECLARE_7
#undef DECLARATIONS_CLEANUP
#endif
