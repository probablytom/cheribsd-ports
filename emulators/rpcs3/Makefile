PORTNAME=	rpcs3
DISTVERSIONPREFIX=	v
DISTVERSION=	0.0.24-14175 # git rev-list --count HEAD
DISTVERSIONSUFFIX=	-g238298f695
CATEGORIES=	emulators wayland

MAINTAINER=	jbeich@FreeBSD.org
COMMENT=	PS3 emulator/debugger
WWW=		https://rpcs3.net/

LICENSE=	APACHE20 ISCL GPLv2 MIT ZLIB
LICENSE_COMB=	multi
LICENSE_FILE_ISCL=	${WRKSRC_cubeb}/LICENSE
LICENSE_FILE_ZLIB=	${WRKSRC_asmjit}/LICENSE.md

BUILD_DEPENDS=	flatc:devel/flatbuffers
LIB_DEPENDS=	libxxhash.so:devel/xxhash \
		libcurl.so:ftp/curl \
		libpng.so:graphics/png \
		libavcodec.so:multimedia/ffmpeg \
		libpugixml.so:textproc/pugixml

ONLY_FOR_ARCHS=	aarch64 amd64
ONLY_FOR_ARCHS_REASON=	requires int128 and SSE2

USE_GITHUB=	yes
GH_ACCOUNT=	RPCS3
GH_TUPLE=	RPCS3:hidapi:hidapi-0.12.0-2-gc2aa9dd:hidapi/3rdparty/hidapi/hidapi \
		RPCS3:llvm-mirror:5521155be5c8:llvm/llvm \
		RPCS3:soundtouch:2.3.1-16-g83cfba6:soundtouch/3rdparty/SoundTouch/soundtouch \
		asmjit:asmjit:06d0bad:asmjit/3rdparty/asmjit/asmjit \
		jbeder:yaml-cpp:yaml-cpp-0.7.0-1-g0b67821:yamlcpp/3rdparty/yaml-cpp/yaml-cpp \
		KhronosGroup:SPIRV-Headers:1.5.4.raytracing.fixed-152-g4995a2f:spirv_headers/3rdparty/SPIRV/SPIRV-Headers \
		KhronosGroup:SPIRV-Tools:v2022.2:spirv_tools/3rdparty/SPIRV/SPIRV-Tools \
		KhronosGroup:glslang:11.10.0:glslang/3rdparty/glslang/glslang \
		kinetiknz:cubeb:cubeb-0.2-1391-gdc511c6:cubeb/3rdparty/cubeb/cubeb \
		wolfSSL:wolfssl:v5.4.0-stable:wolfssl/3rdparty/wolfssl/wolfssl

USES=		cmake compiler:c++17-lang elfctl gl iconv:wchar_t localbase:ldflags \
		openal:soft pkgconfig python:3.4+,build qt:5 xorg
USE_GL=		gl glew
USE_QT=		qmake:build buildtools:build core concurrent dbus gui multimedia svg widgets
USE_XORG=	x11
CMAKE_ON=	CMAKE_SKIP_RPATH ${CURL FFMPEG FLATBUFFERS LIBPNG PUGIXML XXHASH:L:S/^/USE_SYSTEM_/}
CMAKE_ON+=	BUNDLE_SPEEX # only used by cubeb tests
CMAKE_OFF=	USE_NATIVE_INSTRUCTIONS USE_PRECOMPILED_HEADERS
LDFLAGS+=	-Wl,--as-needed # GLU
ELF_FEATURES=	+noaslr,wxneeded:bin/${PORTNAME}

OPTIONS_DEFINE=	ALSA EVDEV FAUDIO PULSEAUDIO JACK SNDIO VULKAN
OPTIONS_DEFAULT=EVDEV FAUDIO PULSEAUDIO JACK SNDIO VULKAN

ALSA_BUILD_DEPENDS=	alsa-lib>0:audio/alsa-lib
ALSA_CMAKE_BOOL=	USE_ALSA

EVDEV_DESC=		libevdev-based joystick support
EVDEV_CMAKE_BOOL=	USE_LIBEVDEV
EVDEV_BUILD_DEPENDS=	evdev-proto>0:devel/evdev-proto
EVDEV_LIB_DEPENDS=	libevdev.so:devel/libevdev

FAUDIO_DESC=		FAudio audio support
FAUDIO_LIB_DEPENDS=	libFAudio.so:audio/faudio
FAUDIO_CMAKE_BOOL=	USE_FAUDIO USE_SYSTEM_FAUDIO

JACK_BUILD_DEPENDS=	jackit>0:audio/jack
JACK_CMAKE_BOOL=	USE_JACK

PULSEAUDIO_BUILD_DEPENDS=pulseaudio>0:audio/pulseaudio
PULSEAUDIO_CMAKE_BOOL=	USE_PULSE

SNDIO_BUILD_DEPENDS=	sndio>0:audio/sndio
SNDIO_CMAKE_BOOL=	USE_SNDIO

VULKAN_DESC=		Vulkan renderer
VULKAN_BUILD_DEPENDS=	${LOCALBASE}/include/vulkan/vulkan.h:graphics/vulkan-headers
VULKAN_LIB_DEPENDS=	libvulkan.so:graphics/vulkan-loader
VULKAN_CMAKE_BOOL=	USE_VULKAN

post-patch:
	@${REINPLACE_CMD} -e '/GIT_BRANCH/s/local_build/master/' \
		 -e '/GIT_FULL_BRANCH/s/local_build/${GH_ACCOUNT}\/${GH_PROJECT}\/master/' \
		-e '/GIT_VERSION/s/local_build/${GH_TAGNAME:C/[^-]*-//}/' \
		${WRKSRC}/${PORTNAME}/git-version.cmake

.include <bsd.port.pre.mk>

# Ignore Mk/bsd.default-versions.mk but respect make.conf(5)
.if !defined(DEFAULT_VERSIONS) || ! ${DEFAULT_VERSIONS:Mllvm*}
LLVM_DEFAULT=	13 # bump if not latest release
.endif
BUILD_DEPENDS+=	clang++${LLVM_DEFAULT}:devel/llvm${LLVM_DEFAULT}
USES:=		${USES:Ncompiler*}
CC=		clang${LLVM_DEFAULT}
CXX=		clang++${LLVM_DEFAULT}
CPP=		clang-cpp${LLVM_DEFAULT}

# XXX Move into separate port and standardize via USES
GH_TUPLE+=	llvm:llvm-project:llvmorg-13.0.1-rc1:libcxx
CXXFLAGS+=	-nostdinc++ -isystem${WRKDIR}/libcxx_prefix/include/c++/v1
LDFLAGS+=	-nostdlib++ -L${WRKDIR}/libcxx_prefix/lib -l:libc++.a -lcxxrt

pre-configure:	bundled-libcxx
bundled-libcxx:
	@${SETENV} ${CONFIGURE_ENV} ${CMAKE_BIN} ${CMAKE_ARGS:M*Ninja*} \
		-DCMAKE_INSTALL_PREFIX:PATH=${WRKDIR}/libcxx_prefix \
		-B ${WRKDIR}/libcxx_build -S ${WRKSRC_libcxx}/libcxx
	@${DO_MAKE_BUILD:NDESTDIR*} install -C ${WRKDIR}/libcxx_build

.include <bsd.port.post.mk>
