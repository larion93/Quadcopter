double PIDCompute(double mError, double kp, double ki, double kd, double Hval, double Lval, unsigned long  tp, double pError, double Ip)
{
  unsigned long tn = millis();
  double dt=(double)(tn-tp);
  double P = (double)kp * mError;
  double D = (double)(kd * (mError - pError) * 1000.0 / dt);
  pError = mError;
  double I = (double)(Ip + ki * mError * dt / 1000.0);
  double U = (double)(P + I + D);
  Ip = I;
  tp=tn;
  if(U>Hval)
  {
    U = Hval;
  }
  else if (U<Lval)
  {
    U = Lval;
  }
  return U;
}
