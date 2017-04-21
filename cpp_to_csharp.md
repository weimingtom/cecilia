* (add) using System;public static class <<<filename>>> {}

* (replace) xxx->xxx  <===> xxx.xxx
* (remove) &xxx  <===> xxx

* (remove) XXX *xxx <====> XXX xxx
* (remove) (XXX *)xxx <====> (XXX)xxx
* (replace) XXX<<<char,int,...>>> *xxx <====> ref XXX xxx
* (replace) XXX<<<array[]>>>> *xxx <====> XXX[] xxx
* (replace) struct XXX *xxx <====> XXX xxx
* (replace) Type xxx; <====> Type xxx = new Type();
* (replace) Type xxx[xxx]; <====> Type[] xxx = new Type[xxx];

* (replace) unsigned char <===> byte
* (replace) unsigned int <====> uint

* (replace) if (!xxx) <====> if (xxx == 0)
* (replace) if (!xxx) <====> if (xxx == NULL)
* (replace) xxx ? 1 : 0 <===> xxx != 0 ? 1 : 0
* (replace) if (xxx) <===> if (xxx != 0)

* (replace) static xxx f() <===> internal static xxx f()
* (replace) xxx f() <===> public static xxx f()

* (replace) in <===> @in
* (replace) out <===> @out
* (replace) NULL <===> null

* (replace) printf (%s, %d) <===> Console.write() ({0}, {0:D})

