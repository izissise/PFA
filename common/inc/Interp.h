#ifndef _INTERP_H_
#define _INTERP_H_

namespace noise
{
  enum class	NoiseQuality
  {
    Low,
      Med,
      High
      };

  // Commonly called Lerp
  inline double	linear(double x, double y, double w)
  {
    return ((1.0 - w) * x) + (w * y);
  }

  inline double cubicSCurve(double nb)
  {
    return (nb * nb * (3.0 - 2.0 * nb));
  }

  inline double quinticSCurve(double nb)
  {
    double nb3 = nb * nb * nb;
    return (6.0 * nb3 * nb * nb) - (15.0 * nb3 * nb) + (10.0 * nb3);
  }

  inline double	interp(double x1, double x2, NoiseQuality q)
  {
    return ((q == NoiseQuality::Low) ?
	    (x1 - x2) :
	    (q == NoiseQuality::Med) ?
	    (cubicSCurve(x1 - x2)) :
	    (quinticSCurve(x1 - x2)));
  }
}

#endif /* _INTERP_H_ */
