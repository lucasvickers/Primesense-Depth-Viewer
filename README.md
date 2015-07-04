####Small project for accessing primesense/kinect using the libfreenect 0.1 drivers
####Heavily relies on the [Cinder-OpenNI](https://github.com/heisters/Cinder-OpenNI) repo

#####Steps
1. install libusb
2. Checkout submodules

#####XCode Tips
- Check the linked .a files in 'Build Settings'
- Check that you copy the dylibs in the 'Copy Bundle Resources' and 'Copy Files' phases

#####VS Tips
- Coming soon

#####It should work, and if if not...

Follow build instructions at:
- [https://github.com/heisters/Cinder-OpenNI](https://github.com/heisters/Cinder-OpenNI)
- [https://github.com/OpenKinect/libfreenect](https://github.com/heisters/Cinder-OpenNI)

#####if the included lib doesn't work right
```
brew install libfreenect1.0::
cd /usr/local
git checkout 8bf7b7e Library/Formula/libfreenect.rb
brew install libfreenect --universal
```
