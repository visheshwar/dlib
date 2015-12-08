// Copyright (C) 2015  Davis E. King (davis@dlib.net)
// License: Boost Software License   See LICENSE.txt for the full license.
#ifndef DLIB_TeNSOR_TOOLS_CPP_
#define DLIB_TeNSOR_TOOLS_CPP_

#include "tensor_tools.h"
#include "../string.h"

namespace dlib { namespace tt
{

// ----------------------------------------------------------------------------------------

    void gemm (
        float beta,
        tensor& dest,
        float alpha,
        const tensor& lhs,
        bool trans_lhs,
        const tensor& rhs,
        bool trans_rhs
    )
    {
#ifdef DLIB_USE_CUDA
        cuda::gemm(beta, dest, alpha, lhs, trans_lhs, rhs, trans_rhs);
#else
        if (beta != 0)
        {
            if (trans_lhs && trans_rhs)
                dest = alpha*trans(mat(lhs))*trans(mat(rhs)) + beta*mat(dest);
            else if (!trans_lhs && trans_rhs)
                dest = alpha*mat(lhs)*trans(mat(rhs)) + beta*mat(dest);
            else if (trans_lhs && !trans_rhs)
                dest = alpha*trans(mat(lhs))*mat(rhs) + beta*mat(dest);
            else
                dest = alpha*mat(lhs)*mat(rhs) + beta*mat(dest);
        }
        else
        {
            if (trans_lhs && trans_rhs)
                dest = alpha*trans(mat(lhs))*trans(mat(rhs));
            else if (!trans_lhs && trans_rhs)
                dest = alpha*mat(lhs)*trans(mat(rhs));
            else if (trans_lhs && !trans_rhs)
                dest = alpha*trans(mat(lhs))*mat(rhs);
            else
                dest = alpha*mat(lhs)*mat(rhs);
        }
#endif
    }

// ----------------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------

    tensor_rand::
    tensor_rand(
        unsigned long long seed
    ) 
#ifdef DLIB_USE_CUDA
    :rnd(seed){}
#else
    {rnd.set_seed(cast_to_string(seed)); }
#endif

    void tensor_rand::
    fill_gaussian (
        tensor& data,
        float mean,
        float stddev
    )
    {
        DLIB_CASSERT(data.size()%2 == 0,"");
#ifdef DLIB_USE_CUDA
        rnd.fill_gaussian(data, mean, stddev);
#else
        for (auto& x : data) 
            x = rnd.get_random_gaussian()*stddev + mean;
#endif
    }

    void tensor_rand::
    fill_uniform (
        tensor& data
    )
    {
#ifdef DLIB_USE_CUDA
        rnd.fill_uniform(data);
#else
        for (auto& x : data) 
            x = rnd.get_random_float();
#endif
    }

// ----------------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------

    void multiply (
        tensor& dest,
        const tensor& src
    )
    {
        DLIB_CASSERT(have_same_dimensions(dest,src) == true,"");
#ifdef DLIB_USE_CUDA
        cuda::multiply(dest, src);
#else
        cpu::multiply(dest, src);
#endif

    }

// ----------------------------------------------------------------------------------------

    void multiply (
        tensor& dest,
        const tensor& src1,
        const tensor& src2
    )
    {
        DLIB_CASSERT(dest.k() == src1.k() && src1.k() == src2.k() &&
            dest.nr() == src1.nr() && src1.nr() == src2.nr() &&
            dest.nc() == src1.nc() && src1.nc() == src2.nc() ,"");
        const long MD = std::max(std::max(dest.num_samples(),src1.num_samples()),src2.num_samples());
        DLIB_CASSERT((dest.num_samples()==1 || dest.num_samples()==MD) &&
                    (src1.num_samples()==1 || src1.num_samples()==MD) &&
                    (src2.num_samples()==1 || src2.num_samples()==MD) ,"");
#ifdef DLIB_USE_CUDA
        cuda::multiply(dest, src1, src2);
#else
        cpu::multiply(dest, src1, src2);
#endif

    }

// ----------------------------------------------------------------------------------------

    void affine_transform(
        tensor& dest,
        const tensor& src,
        const float A,
        const float B
    )
    {
#ifdef DLIB_USE_CUDA
        cuda::affine_transform(dest,src,A,B);
#else
        cpu::affine_transform(dest,src,A,B);
#endif
    }

    void affine_transform(
        tensor& dest,
        const tensor& src1,
        const tensor& src2,
        const float A,
        const float B,
        const float C
    )
    {
#ifdef DLIB_USE_CUDA
        cuda::affine_transform(dest,src1,src2,A,B,C);
#else
        cpu::affine_transform(dest,src1,src2,A,B,C);
#endif
    }

    void affine_transform(
        tensor& dest,
        const tensor& src1,
        const tensor& src2,
        const tensor& src3,
        const float A,
        const float B,
        const float C,
        const float D
    )
    {
#ifdef DLIB_USE_CUDA
        cuda::affine_transform(dest,src1,src2,src3,A,B,C,D);
#else
        cpu::affine_transform(dest,src1,src2,src3,A,B,C,D);
#endif
    }

// ----------------------------------------------------------------------------------------

    void affine_transform(
        tensor& dest,
        const tensor& src,
        const tensor& A,
        const tensor& B
    )
    {
#ifdef DLIB_USE_CUDA
        cuda::affine_transform(dest,src,A,B);
#else
        cpu::affine_transform(dest,src,A,B);
#endif
    }

// ----------------------------------------------------------------------------------------

    void batch_normalize (
        resizable_tensor& dest,
        resizable_tensor& means,
        resizable_tensor& vars,
        const tensor& src,
        const tensor& gamma, 
        const tensor& beta 
    )
    {
#ifdef DLIB_USE_CUDA
        cuda::batch_normalize(dest,means,vars,src,gamma,beta);
#else
        cpu::batch_normalize(dest,means,vars,src,gamma,beta);
#endif
    }

// ----------------------------------------------------------------------------------------

    void batch_normalize_conv (
        resizable_tensor& dest,
        resizable_tensor& means,
        resizable_tensor& vars,
        const tensor& src,
        const tensor& gamma, 
        const tensor& beta 
    )
    {
#ifdef DLIB_USE_CUDA
        cuda::batch_normalize_conv(dest,means,vars,src,gamma,beta);
#else
        cpu::batch_normalize_conv(dest,means,vars,src,gamma,beta);
#endif
    }

// ----------------------------------------------------------------------------------------

    void threshold (
        tensor& data,
        float thresh
    )
    {
#ifdef DLIB_USE_CUDA
        cuda::threshold(data,thresh);
#else
        cpu::threshold(data,thresh);
#endif
    }

// ----------------------------------------------------------------------------------------

    void add(
        float beta,
        tensor& dest,
        float alpha,
        const tensor& src
    )
    {
#ifdef DLIB_USE_CUDA
        cuda::add(beta,dest,alpha,src);
#else
        // TODO
        DLIB_CASSERT(false,"");
#endif
    }

// ----------------------------------------------------------------------------------------

    void add_conv_bias_gradient (
        tensor& grad,
        const tensor& gradient_input
    )
    {
#ifdef DLIB_USE_CUDA
        cuda::add_conv_bias_gradient(grad,gradient_input);
#else
        // TODO
        DLIB_CASSERT(false,"");
#endif
    }

// ----------------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------

    tensor_conv::
    tensor_conv()
    {
    }

    void tensor_conv::
    clear(
    )
    {
#ifdef DLIB_USE_CUDA
        impl.clear();
#else
        // TODO
        DLIB_CASSERT(false,"");
#endif
    }

    void tensor_conv::
    operator() (
        resizable_tensor& output,
        const tensor& data,
        const tensor& filters,
        int stride_y,
        int stride_x
    )
    {
#ifdef DLIB_USE_CUDA
        impl(output, data, filters, stride_y, stride_x);
#else
        // TODO
        DLIB_CASSERT(false,"");
#endif
    }

    void tensor_conv::
    get_gradient_for_data (
        const tensor& gradient_input, 
        const tensor& filters,
        tensor& data_gradient
    )
    {
#ifdef DLIB_USE_CUDA
        impl.get_gradient_for_data(gradient_input, filters, data_gradient);
#else
        // TODO
        DLIB_CASSERT(false,"");
#endif
    }

    void tensor_conv::
    get_gradient_for_filters (
        const tensor& gradient_input, 
        const tensor& data,
        tensor& filters_gradient
    )
    {
#ifdef DLIB_USE_CUDA
        impl.get_gradient_for_filters(gradient_input, data, filters_gradient);
#else
        // TODO
        DLIB_CASSERT(false,"");
#endif
    }

// ----------------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------

    max_pool::
    max_pool (
    )
    {
    }

    void max_pool::
    clear(
    )
    {
#ifdef DLIB_USE_CUDA
        impl.clear();
#else
        // TODO
        DLIB_CASSERT(false,"");
#endif
    }

    void max_pool::
    setup(
        int window_height,
        int window_width,
        int stride_y,
        int stride_x
    )
    {
#ifdef DLIB_USE_CUDA
        impl.setup(window_height, window_width, stride_y, stride_x);
#else
        // TODO
        DLIB_CASSERT(false,"");
#endif
    }

    void max_pool::
    operator() (
        resizable_tensor& dest,
        const tensor& src
    )
    {
#ifdef DLIB_USE_CUDA
        impl(dest, src);
#else
        // TODO
        DLIB_CASSERT(false,"");
#endif
    }

    void max_pool::
    get_gradient(
        const tensor& gradient_input, 
        const tensor& dest,
        const tensor& src,
        tensor& grad 
    )
    {
#ifdef DLIB_USE_CUDA
        impl.get_gradient(gradient_input, dest, src, grad);
#else
        // TODO
        DLIB_CASSERT(false,"");
#endif
    }

// ----------------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------

    void softmax (
        tensor& dest,
        const tensor& src
    )
    {
#ifdef DLIB_USE_CUDA
        cuda::softmax(dest,src);
#else
        cpu::softmax(dest,src);
#endif
    }

    void softmax_gradient (
        tensor& grad,
        const tensor& dest,
        const tensor& gradient_input
    )
    {
#ifdef DLIB_USE_CUDA
        cuda::softmax_gradient(grad, dest, gradient_input);
#else
        cpu::softmax_gradient(grad, dest, gradient_input);
#endif
    }

// ----------------------------------------------------------------------------------------

    void sigmoid (
        tensor& dest,
        const tensor& src
    )
    {
#ifdef DLIB_USE_CUDA
        cuda::sigmoid(dest,src);
#else
        cpu::sigmoid(dest,src);
#endif
    }

    void sigmoid_gradient (
        tensor& grad,
        const tensor& dest,
        const tensor& gradient_input
    )
    {
#ifdef DLIB_USE_CUDA
        cuda::sigmoid_gradient(grad, dest, gradient_input);
#else
        cpu::sigmoid_gradient(grad, dest, gradient_input);
#endif
    }

// ----------------------------------------------------------------------------------------

    void relu (
        tensor& dest,
        const tensor& src
    )
    {
#ifdef DLIB_USE_CUDA
        cuda::relu(dest,src);
#else
        cpu::relu(dest,src);
#endif
    }

    void relu_gradient (
        tensor& grad,
        const tensor& dest,
        const tensor& gradient_input
    )
    {
#ifdef DLIB_USE_CUDA
        cuda::relu_gradient(grad, dest, gradient_input);
#else
        cpu::relu_gradient(grad, dest, gradient_input);
#endif
    }

// ----------------------------------------------------------------------------------------

    void tanh (
        tensor& dest,
        const tensor& src
    )
    {
#ifdef DLIB_USE_CUDA
        cuda::tanh(dest,src);
#else
        cpu::tanh(dest,src);
#endif
    }

    void tanh_gradient (
        tensor& grad,
        const tensor& dest,
        const tensor& gradient_input
    )
    {
#ifdef DLIB_USE_CUDA
        cuda::tanh_gradient(grad, dest, gradient_input);
#else
        cpu::tanh_gradient(grad, dest, gradient_input);
#endif
    }

// ----------------------------------------------------------------------------------------

}}

#endif // DLIB_TeNSOR_TOOLS_CPP_

