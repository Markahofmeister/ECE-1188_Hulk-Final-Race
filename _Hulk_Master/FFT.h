/**
 * @file      FFT.h
 * @brief     Fast Fourier Transform
 * @details   fft() transforms time domain to frequency domain and
 *            ifft() transforms frequency to time domain domain
 * @version   TI-RSLK MAX v1.1
 * @author    Daniel Valvano and Jonathan Valvano
 * @copyright Copyright 2020 by Jonathan W. Valvano, valvano@mail.utexas.edu,
 * @warning   AS-IS
 * @note      For more information see  http://users.ece.utexas.edu/~valvano/
 * @date      July 13, 2020
 ******************************************************************************/

/* Factored discrete Fourier transform, or FFT, and its inverse iFFT */
// Derived from 
// http://www.math.wustl.edu/~victor/mfmm/index.html#mfmm-software
// Mathematics for Multimedia, ISBN 978-0-8176-4879-4, was formerly managed by Elsevier/Academic Press.
// MSP432 available RAM limits size to N=2048 points
#include <math.h>
#include <stdlib.h>
/*!
 * @defgroup Math
 * @brief
 * @{*/

/*!
 * @brief a complex number has a real and imaginary part
 */
struct complex{
  float Real; 
  float Imag;
}; 
typedef struct complex complex_t;

/**
 * \brief PI is ratio of circumference to diameter of a circle
 */
#ifndef PI
#define PI	3.14159265358979323846264338327950288
#endif

/**
 * discrete Fast Fourier Transform,
 * Converts time to frequency domain                             <br>
 * Assume the input is real data sampled at fs                   <br>
 * Place n measurements into the real part of array v            <br>
 * Set the imaginary part of array v to 0                        <br>
 * Call fft() and the results are returned back in array v       <br>
 * If the input is real only, then the transform will contain    <br>
 *   Index 0 <= k < n/2 complex components of frequency f=k/fs   <br>
 *   E.g., k=0 represents the DC component of the input          <br>
 *   Index n/2 <= k < n are complex conjugates of the first half <br>
 * @param v array of input data in complex form
 * @param n size of the two arrays 2, 4, 8, ..., 2048
 * @param tmp temporary array of data in complex form
 * @return none
 * @note n is a power of two from 2 to 2048
 * @brief fft
 */
void fft(complex_t *v, int n, complex_t *tmp);


/**
 * discrete Inverse Fast Fourier Transform,
 * Converts frequency to time domain                                        <br>
 * Assume the input is complex data with components k=0 to n-1              <br>
 * Index 0 <= k < n-1 complex components of frequency f=k/fs                <br>
 *   E.g., k=0 represents the DC component of the input                     <br>
 * Place n complex values into the array v                                  <br>
 * Typically the index n/2 <= k < n are complex conjugates of the first half<br>
 * Call ifft() and the results are returned back in array v                 <br>
 * If the input is real only, then the transform will contain               <br>
 * @param v array of input data in complex form
 * @param n size of the two arrays 2, 4, 8, ..., 2048
 * @param tmp temporary array of data in complex form
 * @return none
 * @note n is a power of two from 2 to 2048
 * @brief Inverse fft
 */
void ifft(complex_t *v, int n, complex_t *tmp);


