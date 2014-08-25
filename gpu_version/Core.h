#include<cfloat>
#include<iostream>
#include<sstream>
#include<stdexcept>

#include "Field.h"

#ifdef __CUDACC__
#define __common__ __host__ __device__
#else
#define __common__
#endif

// Modes

struct Initial;
struct GPU;
struct CPU;

struct Eval;

// Field expressions

template<typename Mode, typename FieldType>
struct FieldExpr;

template<typename FieldType>
class FieldExpr<Eval, FieldType> {
    private:
        double* array;

    public:
        Dimensions dim;
        FieldExpr(double* arrayArg, Dimensions dimArg) : array(arrayArg), dim(dimArg) {}

        __common__ double eval(int x, int y, int z) const {
            return array[z * dim.y + y * dim.x + x];
        }
};

template<typename FieldType>
class FieldExpr<Initial, FieldType> {
    private:
        const FieldType & field;

    public:
        FieldExpr<Eval, FieldType> typedef CPUType;
        FieldExpr<Eval, FieldType> typedef GPUType;

        FieldExpr(const FieldType & fieldArg) : field(fieldArg) {}

        bool cpu_ready() const {
            return field.cpu_ready();
        }

        const CPUType cpu_init() const {
            return CPUType(field.get_data(), field.dim);
        }

        #ifdef __CUDACC__
        bool gpu_ready() const {
            return field.gpu_ready();
        }

        const GPUType gpu_init() const {
            return GPUType(field.get_data_gpu(), field.dim);
        }
        #endif
};

// Constant expressions

struct ConstExpr {
    ConstExpr typedef CPUType;
    ConstExpr typedef GPUType;

    const double value;

    ConstExpr(const double valueArg) : value(valueArg) {}

    bool cpu_ready() const {
        return true;
    }

    CPUType cpu_init() const {
        return *this;
    }

    #ifdef __CUDACC__
    bool gpu_ready() const {
        return true;
    }

    GPUType gpu_init() const {
        return *this;
    }
    #endif

    __common__ double eval(int x, int y, int z) const {
        return value;
    }
};

// Binary expressions

template<typename Mode, typename Functor, typename SubExpr1, typename SubExpr2>
class BinExpr;

template<typename Functor, typename SubExpr1, typename SubExpr2>
class BinExpr<Initial, Functor, SubExpr1, SubExpr2> {
    private:
        const SubExpr1 subExpr1;
        const SubExpr2 subExpr2;

    public:
        BinExpr<Eval, Functor, typename SubExpr1::CPUType, typename SubExpr2::CPUType> typedef CPUType;
        BinExpr<Eval, Functor, typename SubExpr1::GPUType, typename SubExpr2::GPUType> typedef GPUType;

        BinExpr(const SubExpr1 & subExpr1Arg, const SubExpr2 & subExpr2Arg)
            : subExpr1(subExpr1Arg), subExpr2(subExpr2Arg) {}

        bool cpu_ready() const {
            return subExpr1.cpu_ready() && subExpr2.cpu_ready();
        }

        const CPUType cpu_init() const {
            return CPUType(subExpr1.cpu_init(), subExpr2.cpu_init());
        }

        #ifdef __CUDACC__
        bool gpu_ready() const {
            return subExpr1.gpu_ready() && subExpr2.gpu_ready();
        }

        const GPUType gpu_init() const {
            return GPUType(subExpr1.gpu_init(), subExpr2.gpu_init());
        }
        #endif
};

template<typename Functor, typename SubExpr1, typename SubExpr2>
class BinExpr<Eval, Functor, SubExpr1, SubExpr2> {
    private:
        const SubExpr1 subExpr1;
        const SubExpr2 subExpr2;
        const Functor functor;

    public:
        BinExpr(const SubExpr1 & subExpr1Arg, const SubExpr2 & subExpr2Arg)
            : subExpr1(subExpr1Arg), subExpr2(subExpr2Arg), functor() {}

        __common__ double eval(int x, int y, int z) const {
            return functor(subExpr1.eval(x, y, z), subExpr2.eval(x, y, z));
        }
};

// Printing Fields to std::cout

template<typename FieldType>
std::ostream & core_write_field(std::ostream & os, FieldType & field)
{
    typename FieldExpr<Initial, FieldType>::CPUType expr = FieldExpr<Initial, FieldType>(field).cpu_init();
    for (int z = 0; z < expr.dim.z; z++) {
        os << "z = " << z << std::endl;
        for (int y = 0; y < expr.dim.y; y++) {
            for (int x = 0; x < expr.dim.x; x++) {
                os << expr.eval(x, y, z) << " ";
            }
            os << std::endl;
        }
        os << std::endl;
    }

    return os;
}

// Assignment

#ifdef __CUDACC__
template<typename Expr>
__global__ void gpu_assign_kernel(double* data, Dimensions dim, Expr expr) {
    const int x = blockIdx.x * blockDim.x + threadIdx.x;
    const int y = blockIdx.y * blockDim.y + threadIdx.y;

    for(int z = 0; z < dim.z; z++) {
        if(x < dim.x && y < dim.y) {
            data[z * dim.y + y * dim.x + x] = expr.eval(x, y, z);
        }
    }

}
#endif

template<typename FieldType, typename Expr>
void core_assign(FieldType & field, const Expr & rhs) {
    if (false) {}

    #ifdef __CUDACC__
    else if (field.gpu_ready() && rhs.gpu_ready()) {
        int blockDim = 32;

        int xGDim = field.dim.x / blockDim + ((field.dim.x % blockDim) > 0 ? 1 : 0);
        int yGDim = field.dim.y / blockDim + ((field.dim.y % blockDim) > 0 ? 1 : 0);

        dim3 dimBlock(blockDim, blockDim);
        dim3 dimGrid(xGDim, yGDim);

        typename Expr::GPUType typedef GPUType;

        gpu_assign_kernel<GPUType><<<dimGrid, dimBlock, 0>>> (
                field.get_data_gpu(),
                field.dim,
                rhs.gpu_init());

        checkCuda(cudaDeviceSynchronize());
    }
    #endif

    else if (field.cpu_ready() && rhs.cpu_ready()) {
        typename Expr::CPUType expr = rhs.cpu_init();
        double* data = field.get_data();
        for (int z = 0; z <  field.dim.z; z++) {
            for (int y = 0; y < field.dim.y; y++) {
                for (int x = 0; x < field.dim.x; x++) {
                    data[z * field.dim.y + y * field.dim.x + x] = expr.eval(x, y, z);
                }
            }
        }
    } else {
        throw std::runtime_error("lhs and rhs elements are not all available together on a device.");
    }
}

// Operators

struct SumOp {
    __common__ double operator() (double lhs, double rhs) const {
        return lhs + rhs;
    }
};

struct MultOp {
    __common__ double operator() (double lhs, double rhs) const {
        return lhs * rhs;
    }
};
