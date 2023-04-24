PORTNAME=	vala
DISTVERSION=	${_VALA_VERSION}
PORTEPOCH=	1
CATEGORIES=	lang gnome
MASTER_SITES=	GNOME

MAINTAINER=	desktop@FreeBSD.org
COMMENT=	Programming language and compiler that converts Vala code into C code
WWW=		https://wiki.gnome.org/Projects/Vala

LICENSE=	LGPL21

PORTSCOUT=	limitw:1,even

USES=		bison cpe gettext-runtime gmake gnome libtool \
		localbase pathfix pkgconfig tar:xz\
		vala:no_depend
USE_GNOME=	glib20 gnomeprefix libxslt:build
CPE_VENDOR=	gnome
GNU_CONFIGURE=	yes
USE_LDCONFIG=	yes
CONFIGURE_ARGS=	--disable-valadoc
INSTALL_TARGET=	install-strip

PLIST_SUB=	VALA_LIB_VERSION=${_VALA_LIB_VERSION}

.include <bsd.port.pre.mk>

.if ${ARCH} == aarch64c
# Patch the vala distribution for the glib changes required on CHERI; use of gintptr rather than gsize when calling `g_once_init`.
# As the vala compiler is written in vala, to upstream this change it will have to be made to the vala sources.
post-patch:
	@find ${WRKSRC} -type f -name '*.c' -exec ${REINPLACE_CMD} -e 's/static volatile gsize/static volatile gintptr/g' {} \;
	@${REINPLACE_CMD} -e 's/volatile gsize\*/volatile gintptr\*/g' -e 's/\(gsize\) val/\(gintptr\) val/g' ${WRKSRC}/vala/valacodecontext.c
.endif

.include <bsd.port.mk>
