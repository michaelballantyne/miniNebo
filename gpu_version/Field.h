#ifdef __CUDACC__
#include<cuda_runtime.h>
#endif

// CUDA error handling

#ifdef __CUDACC__
#define checkCuda(ans) { gpuAssert((ans), __FILE__, __LINE__); }
inline void gpuAssert(cudaError_t code, char *file, int line)
{
   if (code != cudaSuccess)
   {
       std::ostringstream s;
       s << "cuda error " << code << " " << file << ":" << line << std::endl;
       throw std::runtime_error(s.str());
   }
}
#endif

// Dimensions

struct Dimensions {
    const int x;
    const int y;
    const int z;

    Dimensions(int xdim, int ydim, int zdim)
        : x(xdim), y(ydim), z(zdim) {}
};


// Field

enum Device {DEVICE_CPU, DEVICE_GPU};

template<typename Location>
class TypedField {
    private:
        double* cpu_array;

        #ifdef __CUDACC__
        double* gpu_array;
        #endif

    public:
        Location typedef location;

        const Dimensions dim;

        TypedField(const Dimensions dimArg, Device device)
            : dim(dimArg)
        {
            if (device == DEVICE_CPU) {
                cpu_array = new double[dim.x * dim.y * dim.z];
            }

            #ifdef __CUDACC__
            else if (device == DEVICE_GPU) {
                checkCuda(cudaMalloc(&gpu_array, sizeof(double) * dim.x * dim.y * dim.z));
            }
            #endif

            else {
                throw std::invalid_argument("Invalid device.");
            }
        }

        ~TypedField() {
            if (cpu_array != NULL) {
                delete cpu_array;
            }

            #ifdef __CUDACC__
            if (gpu_array != NULL) {
                checkCuda(cudaFree(gpu_array));
            }
            #endif
        }

        bool cpu_ready() const {
            return cpu_array != NULL;
        }

        double* get_data() const {
            if (!cpu_ready()) {
                throw std::runtime_error("Data not available on the CPU.");
            }

            return cpu_array;
        }

        #ifdef __CUDACC__
        bool gpu_ready() const {
            return gpu_array != NULL;
        }

        double* get_data_gpu() const {
            if (!gpu_ready()) {
                throw std::runtime_error("Data not available on the GPU.");
            }

            return gpu_array;
        }

        void copy_cpu_to_gpu() {
            if (gpu_array == NULL) {
                checkCuda(cudaMalloc(&gpu_array, sizeof(double) * dim.x * dim.y * dim.z));
            }

            checkCuda(cudaMemcpy(gpu_array, cpu_array, sizeof(double) * dim.x * dim.y * dim.z, cudaMemcpyHostToDevice));
        }

        void copy_gpu_to_cpu() {
            if (cpu_array == NULL) {
                cpu_array = new double[dim.x * dim.y * dim.z];
            }

            checkCuda(cudaMemcpy(cpu_array, gpu_array, sizeof(double) * dim.x * dim.y * dim.z, cudaMemcpyDeviceToHost));
        }
        #endif
};

struct Untyped;
TypedField<Untyped> typedef Field;
