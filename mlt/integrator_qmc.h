#pragma once 

#include "integrator_pt.h"
#include "rnd_qmc.h"

#include <memory>
#include <limits>

struct LensElementInterface  // not nessesary part of QMC. Just implemented here for test cases, could be moved in main class further
{
  float curvatureRadius;
  float thickness;
  float eta;
  float apertureRadius;
};

class IntegratorQMC : public Integrator
{
public:

  IntegratorQMC(int a_maxThreads, int a_spectral_mode, std::vector<uint32_t> a_features);

  float  GetRandomNumbersSpec(uint tid, RandomGen* a_gen) override;
  float  GetRandomNumbersTime(uint tid, RandomGen* a_gen) override;
  float4 GetRandomNumbersLens(uint tid, RandomGen* a_gen) override;
  float4 GetRandomNumbersMats(uint tid, RandomGen* a_gen, int a_bounce) override;
  float4 GetRandomNumbersLgts(uint tid, RandomGen* a_gen, int a_bounce) override;
  float  GetRandomNumbersMatB(uint tid, RandomGen* a_gen, int a_bounce, int a_layer) override;

  EyeRayData SampleCameraRay(RandomGen* pGen, uint tid) override;
  uint       RandomGenId(uint tid) override;

  void   kernel_ContributeToImage(uint tid, const uint* rayFlags, uint channels, const float4* a_accumColor, const RandomGen* gen,
                                  const uint* in_pakedXY, const float4* wavelengths, float* out_color) override;

  void   PathTraceBlock(uint pixelsNum, uint channels, float* out_color, uint a_passNum) override;
  unsigned int m_qmcTable[qmc::QRNG_DIMENSIONS][qmc::QRNG_RESOLUTION];
  
  void EnableQMC();
  uint m_qmcMatDim = 0;
  uint m_qmcLgtDim = 0;
  uint m_qmcSpdDim = 0;
  uint m_qmcMotionDim = 0;
  uint m_qmcDofDim = 0;
  
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////
  // not nessesary part of QMC. Just implemented here for test cases, could be moved in main class further // 
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////
  uint m_enableOpticSim = 1;
  std::vector<LensElementInterface> lines;
  float2 m_physSize;
  float  m_diagonal;
  float  m_aspect;

  inline float LensRearZ()      const { return lines[0].thickness; }
  inline float LensRearRadius() const { return lines[0].apertureRadius; }         

  bool IntersectSphericalElement(float radius, float zCenter, float3 rayPos, float3 rayDir, 
                                 float *t, float3 *n) const;

  bool TraceLensesFromFilm(float3& rayPos, float3& rayDir) const;
};
