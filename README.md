PlatPal
=======

Documentation
-------------
Please see our USENIX Security'17 [paper](https://sslab.gtisc.gatech.edu/assets/papers/2017/xu:platpal.pdf)

Stay tuned!
-----------
Building and running the PlatPal Adobe Reader plugin is rather complicated
so please allow me a bit more time to document down the procedure.

As a starting point, the script `acrobat/driver/build/run.py` should be
self-explanatory. However, you do need to acquire a private Adobe signing
key to finish up the building process. Details can be found in the official
[Adobe SDK documentation](https://help.adobe.com/en_US/acrobat/acrobat_dc_sdk/2015/HTMLHelp/index.html#t=Acro12_MasterBook/Reader_Plugins/Reader_enablement.htm)at 

In addition, although the plugin is written in a cross-platform manner, the
building process on Mac OS and Window is totally different. Python scripts
are available (`build/mac.py`, `build/win.py` respectively) to allow you to
build the plugin all from your command line and thus, avoiding Xcode and
Visual Studios.
