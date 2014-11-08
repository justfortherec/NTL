

#include <NTL/ZZ_pE.h>

#include <NTL/new.h>

NTL_START_IMPL

ZZ_pEInfoT::ZZ_pEInfoT(const ZZ_pX& NewP)
{
   build(p, NewP);

   _card_base = ZZ_p::modulus();
   _card_exp = deg(NewP);
}

const ZZ& ZZ_pE::cardinality()
{
   if (!ZZ_pEInfo) Error("ZZ_pE::cardinality: undefined modulus");

   do { // NOTE: thread safe lazy init
      Lazy<ZZ>::Builder builder(ZZ_pEInfo->_card);
      if (!builder) break;
      power(*builder, ZZ_pEInfo->_card_base, ZZ_pEInfo->_card_exp);
   } while (0);
      
   return ZZ_pEInfo->_card.value();
}





NTL_THREAD_LOCAL 
SmartPtr<ZZ_pEInfoT> ZZ_pEInfo = 0; 


void ZZ_pE::init(const ZZ_pX& p)
{
   ZZ_pEContext c(p);
   c.restore();
}


void ZZ_pEContext::save()
{
   ptr = ZZ_pEInfo;
}

void ZZ_pEContext::restore() const
{
   ZZ_pEInfo = ptr;
}


ZZ_pEBak::~ZZ_pEBak()
{
   if (MustRestore) c.restore();
}

void ZZ_pEBak::save()
{
   c.save();
   MustRestore = true;
}


void ZZ_pEBak::restore()
{
   c.restore();
   MustRestore = false;
}


const ZZ_pE& ZZ_pE::zero()
{
   static ZZ_pE z(INIT_NO_ALLOC);
   return z;
}




istream& operator>>(istream& s, ZZ_pE& x)
{
   ZZ_pX y;

   NTL_INPUT_CHECK_RET(s, s >> y);
   conv(x, y);

   return s;
}

void div(ZZ_pE& x, const ZZ_pE& a, const ZZ_pE& b)
{
   ZZ_pE t;

   inv(t, b);
   mul(x, a, t);
}

void div(ZZ_pE& x, const ZZ_pE& a, long b)
{
   NTL_ZZ_pRegister(B);
   B = b;
   inv(B, B);
   mul(x, a, B);
}

void div(ZZ_pE& x, const ZZ_pE& a, const ZZ_p& b)
{
   NTL_ZZ_pRegister(B);
   B = b;
   inv(B, B);
   mul(x, a, B);
}

void div(ZZ_pE& x, long a, const ZZ_pE& b)
{
   ZZ_pE t;
   inv(t, b);
   mul(x, a, t);
}

void div(ZZ_pE& x, const ZZ_p& a, const ZZ_pE& b)
{
   ZZ_pE t;
   inv(t, b);
   mul(x, a, t);
}



void inv(ZZ_pE& x, const ZZ_pE& a)
{
   InvMod(x._ZZ_pE__rep, a._ZZ_pE__rep, ZZ_pE::modulus());
}

NTL_END_IMPL
