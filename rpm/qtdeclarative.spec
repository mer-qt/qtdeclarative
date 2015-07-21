Name:       qt5-qtdeclarative
Summary:    Qt Declarative library
Version:    5.0.2
Release:    1%{?dist}
Group:      Qt/Qt
License:    LGPLv2.1 with exception or GPLv3
URL:        http://qt.nokia.com
Source0:    %{name}-%{version}.tar.bz2
BuildRequires:  qt5-qtcore-devel
BuildRequires:  qt5-qtgui-devel >= 5.2.1+git24
BuildRequires:  qt5-qtnetwork-devel
BuildRequires:  qt5-qtsql-devel
BuildRequires:  qt5-qttest-devel
BuildRequires:  qt5-qtxmlpatterns-devel
BuildRequires:  qt5-qmake
BuildRequires:  fdupes
BuildRequires:  python
BuildRequires:  gdb
Obsoletes: qt5-qtjsbackend <= 5.1.0+git4
Provides: qt5-qtjsbackend > 5.1.0+git4

%description
Qt is a cross-platform application and UI framework. Using Qt, you can
write web-enabled applications once and deploy them across desktop,
mobile and embedded systems without rewriting the source code.
.
This package contains the Declarative library


%package devel
Summary:    Qt Declarative - development files
Group:      Qt/Qt
Requires:   %{name} = %{version}-%{release}
Requires:   qt5-qtsql-devel
Requires:   qt5-qtnetwork-devel

%description devel
Qt is a cross-platform application and UI framework. Using Qt, you can
write web-enabled applications once and deploy them across desktop,
mobile and embedded systems without rewriting the source code.
.
This package contains the Declarative library development files

%package doc
Summary:    Qt Declarative - documentation
Group:      Qt/Qt
Requires:   %{name} = %{version}-%{release}

%description doc
This package contains the Qt Declarative documentation

%package qtquicktest
Summary:    Qt Declarative QtQuickTest library
Group:      Qt/Qt
Requires:   %{name} = %{version}-%{release}
Requires(post):     /sbin/ldconfig
Requires(postun):   /sbin/ldconfig

%description qtquicktest
This package contains the QtQuickTest library for QtDeclarative module

%package qtquicktest-devel
Summary:    Qt Declarative QtQuickTest - development files
Group:      Qt/Qt
Requires:   %{name} = %{version}-%{release}
Requires:   %{name}-devel = %{version}-%{release}
Requires:   qt5-qtdeclarative-qtquicktest = %{version}-%{release}

%description qtquicktest-devel
This package contains the development headers for QtQuickTest library

%package qtquick
Summary:    Qt Declarative - QtQuick library
Group:      Qt/Qt
Requires:   %{name} = %{version}-%{release}

%description qtquick
This package contains the QtQuick QML support library

%package qtquick-devel
Summary:    Qt Declarative - QtQuick development files
Group:      Qt/Qt
Requires:   %{name} = %{version}-%{release}
Requires:   qt5-qtdeclarative-qtquick = %{version}-%{release}

%description qtquick-devel
This package contains the development headers for legacy QtQuick 1
QML support library

%package qtquick-doc
Summary:    Qt Declarative - QtQuick documentation
Group:      Qt/Qt
Requires:   %{name} = %{version}-%{release}
Requires:   qt5-qtdeclarative-qtquick = %{version}-%{release}

%description qtquick-doc
This package contains the Qt Declarative QtQuick documentation

%package qtquickparticles
Summary:    Qt Declarative - QtQuick Particles library
Group:      Qt/Qt
Requires:   %{name} = %{version}-%{release}

%description qtquickparticles
This package contains the QtQuick Particles support library

%package qtquickparticles-devel
Summary:    Qt Declarative - QtQuick Particles development files
Group:      Qt/Qt
Requires:   %{name} = %{version}-%{release}
Requires:   qt5-qtdeclarative-qtquickparticles = %{version}-%{release}

%description qtquickparticles-devel
This package contains the development headers for QtQuickParticles
QML support library


%package qtdeclarativetools-devel
Summary:    Qt Declarative QtQmlDevTools - development files
Group:      Qt/Qt
Requires:   %{name} = %{version}-%{release}
Requires:   %{name}-devel = %{version}-%{release}
Requires:   qt5-qtdeclarative-devel = %{version}-%{release}

%description qtdeclarativetools-devel
This package contains the development headers for QtQmlDevTools



#### Small plugin and import packages

%package import-folderlistmodel
Summary:    Qt Declarative folderlistmodel plugin
Group:      Qt/Qt
Requires:   %{name} = %{version}-%{release}

%description import-folderlistmodel
This package provides the QtQml folderlistmodel plugin

%package import-settings
Summary:    Qt Declarative settings plugin
Group:      Qt/Qt
Requires:   %{name} = %{version}-%{release}

%description import-settings
This package provides the %{summary}

%package import-localstorageplugin
Summary:    Qt LocalStorage plugin
Requires:   %{name} = %{version}-%{release}

%description import-localstorageplugin
This package provided the Qt LocalStorage plugin

%package plugin-qmlinspector
Summary:    Qt Declarative QML inspector plugin
Group:      Qt/Qt
Requires:   %{name} = %{version}-%{release}

%description plugin-qmlinspector
This package provides the QML inspector plugin

%package plugin-accessible
Summary:    Qt Declarative accessible plugin
Group:      Qt/Qt
Requires:   %{name} = %{version}-%{release}

%description plugin-accessible
This package provides the QML accessible plugin


%package import-qtquick2plugin
Summary:    Qt Declarative QtQuick 2 support plugin
Group:      Qt/Qt
Requires:   %{name} = %{version}-%{release}

%description import-qtquick2plugin
This package provides the QtQuick 2 support plugin

%package import-qttest
Summary:    Qt Declarative QtTest plugin
Group:      Qt/Qt
Requires:   %{name} = %{version}-%{release}

%description import-qttest
This package provides the QtQml QtTest plugin

%package import-particles2
Summary:    Qt Declarative Particles plugin
Group:      Qt/Qt
Requires:   %{name} = %{version}-%{release}

%description import-particles2
This package provides the QtQml Particles.2 plugin

%package import-window2
Summary:    Qt Declarative Window plugin
Group:      Qt/Qt
Requires:   %{name} = %{version}-%{release}

%description import-window2
This package provides the QtQml Window.2 plugin

%package import-models2
Summary:    Qt Declarative models plugin
Group:      Qt/Qt
Requires:   %{name} = %{version}-%{release}

%description import-models2
This package provides the QtDeclarative models plugin for QtQuick 2.0

%package import-xmllistmodel
Summary:    Qt Declarative XmlListModel plugin
Group:      Qt/Qt
Requires:   %{name} = %{version}-%{release}

%description import-xmllistmodel
This package provides the QtDeclarative XmlListModel plugin for QtQuick 2.0

%package qmlscene
Summary:    QML scene viewer
Group:      Qt/Qt
Requires:   %{name} = %{version}-%{release}

%description qmlscene
This package contains the QML viewer for QtQuick 2.0 files.

%package tool-qml
Summary:    QML runtime binary
Group:      Qt/Qt
Requires:   %{name} = %{version}-%{release}

%description tool-qml
This package contains the %{summary} tool

%package tool-qmlimportscanner
Summary:    QML runtime binary
Group:      Qt/Qt
Requires:   %{name} = %{version}-%{release}

%description tool-qmlimportscanner
This package contains the %{summary} tool

%package devel-tools
Summary:    QML development tools
Group:      Qt/Qt
Requires:   %{name} = %{version}-%{release}

%description devel-tools
This package contains QML debugging and development tools





%prep
%setup -q -n %{name}-%{version}

%build
export QTDIR=/usr/share/qt5
touch .git

%ifarch %arm
# to enable JIT, we need to enable thumb, as it is the only supported
# configuration for JIT on ARM. unfortunately, we are not currently in the right
# frame of mind to be able to deal with a full thumb transition, so we need to
# hack it in.
#
# OBS forces -mno-thumb, so first step, we need to remove that, and then add our
# own thumb argument. we can't do this in the .pro, as it won't propegate. we
# can't do it in .qmake.conf, because that's loaded too early. -after is *just*
# the right place: it's after everything has happened except for
# default_post.prf, which sets up the real QMAKE_C{XX}FLAGS, so brutally abuse
# it to acomplish our evil goals.
%qmake5 \
    QT.widgets.name= DEFINES+=QT_NO_WIDGETS \
    -after \
    QMAKE_CFLAGS_RELEASE-=-mno-thumb     QMAKE_CFLAGS_DEBUG-=-mno-thumb \
    QMAKE_CXXFLAGS_RELEASE-=-mno-thumb   QMAKE_CXXFLAGS_DEBUG-=-mno-thumb \
    QMAKE_CFLAGS_RELEASE+=-mthumb        QMAKE_CFLAGS_DEBUG+=-mthumb \
    QMAKE_CXXFLAGS_RELEASE+=-mthumb      QMAKE_CXXFLAGS_DEBUG+=-mthumb
%else
%qmake5 QT.widgets.name= DEFINES+=QT_NO_WIDGETS
%endif

make %{?_smp_mflags}
make docs


%install
rm -rf %{buildroot}
%qmake5_install
# Fix wrong path in pkgconfig files
find %{buildroot}%{_libdir}/pkgconfig -type f -name '*.pc' \
-exec perl -pi -e "s, -L%{_builddir}/?\S+,,g" {} \;
# Fix wrong path in prl files
find %{buildroot}%{_libdir} -type f -name '*.prl' \
-exec sed -i -e "/^QMAKE_PRL_BUILD_DIR/d;s/\(QMAKE_PRL_LIBS =\).*/\1/" {} \;
# Remove unneeded .la files
rm -f %{buildroot}/%{_libdir}/*.la

# We don't need qt5/Qt/
rm -rf %{buildroot}/%{_includedir}/qt5/Qt

# Manually copy qmldevtools static library
cp lib/libQt5QmlDevTools.a %{buildroot}/%{_libdir}
%fdupes %{buildroot}/%{_libdir}
%fdupes %{buildroot}/%{_includedir}


# Copy docs
mkdir -p %{buildroot}/%{_docdir}/qt5
cp -R doc/qtqml %{buildroot}/%{_docdir}/qt5
cp -R doc/qtqml.qch %{buildroot}/%{_docdir}/qt5
cp -R doc/qtquick %{buildroot}/%{_docdir}/qt5
cp -R doc/qtquick.qch %{buildroot}/%{_docdir}/qt5


#### Pre/Post section

%post
/sbin/ldconfig
%postun
/sbin/ldconfig

%post qtquicktest
/sbin/ldconfig
%postun qtquicktest
/sbin/ldconfig

%post qtquick
/sbin/ldconfig
%postun qtquick
/sbin/ldconfig


%post qtquickparticles
/sbin/ldconfig
%postun qtquickparticles
/sbin/ldconfig





#### File section


%files
%defattr(-,root,root,-)
%{_libdir}/libQt5Qml.so.5
%{_libdir}/libQt5Qml.so.5.*

# FIXME: the provided .pc file is empty!
# Find out what gives and find a clean resolution
%files devel
%defattr(-,root,root,-)
%{_libdir}/libQt5Qml.so
%{_libdir}/libQt5Qml.prl
%{_libdir}/pkgconfig/Qt5Qml.pc
%{_includedir}/qt5/QtQml/
%{_datadir}/qt5/mkspecs/modules/qt_lib_qml.pri
%{_datadir}/qt5/mkspecs/modules/qt_lib_qml_private.pri
%{_libdir}/cmake/

%files doc
%{_docdir}/qt5/qtqml/
%{_docdir}/qt5/qtqml.qch

%files qtquick
%defattr(-,root,root,-)
%{_libdir}/libQt5Quick.so.5
%{_libdir}/libQt5Quick.so.5.*

%files qtquick-devel
%defattr(-,root,root,-)
%{_libdir}/libQt5Quick.so
%{_libdir}/libQt5Quick.prl
%{_libdir}/pkgconfig/Qt5Quick.pc
%{_includedir}/qt5/QtQuick/
%{_datadir}/qt5/mkspecs/modules/qt_lib_quick.pri
%{_datadir}/qt5/mkspecs/modules/qt_lib_quick_private.pri

%files qtquick-doc
%{_docdir}/qt5/qtquick/
%{_docdir}/qt5/qtquick.qch





%files qmlscene
%defattr(-,root,root,-)
%{_qt5_bindir}/qmlscene

%files tool-qml
%defattr(-,root,root,-)
%{_qt5_bindir}/qml

%files tool-qmlimportscanner
%defattr(-,root,root,-)
%{_qt5_bindir}/qmlimportscanner

%files devel-tools
%defattr(-,root,root,-)
%{_qt5_bindir}/qmlplugindump
%{_qt5_bindir}/qmlprofiler
%{_qt5_bindir}/qmltestrunner
%{_qt5_bindir}/qmlmin
%{_qt5_bindir}/qmlbundle
%if ! 0%{?qt5_release_build}
%{_qt5_bindir}/qmljs
%endif


%files import-folderlistmodel
%defattr(-,root,root,-)
%{_libdir}/qt5/qml/Qt/labs/folderlistmodel/*

%files import-settings
%defattr(-,root,root,-)
%{_libdir}/qt5/qml/Qt/labs/settings/*

%files import-localstorageplugin
%defattr(-,root,root,-)
%{_libdir}/qt5/qml/QtQuick/LocalStorage/

%files plugin-qmlinspector
%defattr(-,root,root,-)
%{_libdir}/qt5/plugins/qmltooling/*

%files plugin-accessible
%defattr(-,root,root,-)
%{_libdir}/qt5/plugins/accessible/libqtaccessiblequick.so

%files import-qttest
%defattr(-,root,root,-)
%{_libdir}/qt5/qml/QtTest/

%files import-qtquick2plugin
%defattr(-,root,root,-)
%{_libdir}/qt5/qml/QtQuick.2/

%files import-particles2
%defattr(-,root,root,-)
%{_libdir}/qt5/qml/QtQuick/Particles.2/

%files import-window2
%defattr(-,root,root,-)
%{_libdir}/qt5/qml/QtQuick/Window.2/

%files import-models2
%defattr(-,root,root,-)
%{_libdir}/qt5/qml/QtQml/Models.2/

%files import-xmllistmodel
%defattr(-,root,root,-)
%{_libdir}/qt5/qml/QtQuick/XmlListModel/



%files qtquicktest
%defattr(-,root,root,-)
%{_libdir}/libQt5QuickTest.so.5
%{_libdir}/libQt5QuickTest.so.5.*

%files qtquicktest-devel
%defattr(-,root,root,-)
%{_includedir}/qt5/QtQuickTest/
%{_libdir}/libQt5QuickTest.so
%{_libdir}/libQt5QuickTest.prl
%{_libdir}/pkgconfig/Qt5QuickTest.pc
%{_datadir}/qt5/mkspecs/modules/qt_lib_qmltest.pri
%{_datadir}/qt5/mkspecs/modules/qt_lib_qmltest_private.pri

%files qtquickparticles
%defattr(-,root,root,-)
%{_libdir}/libQt5QuickParticles.so.5
%{_libdir}/libQt5QuickParticles.so.5.*

%files qtquickparticles-devel
%defattr(-,root,root,-)
%{_includedir}/qt5/QtQuickParticles/
%{_libdir}/libQt5QuickParticles.so
%{_libdir}/libQt5QuickParticles.prl
%{_libdir}/pkgconfig/Qt5QuickParticles.pc
%{_datadir}/qt5/mkspecs/modules/qt_lib_quickparticles_private.pri

%files qtdeclarativetools-devel
%defattr(-,root,root,-)
%{_libdir}/libQt5QmlDevTools.a
%{_libdir}/libQt5QmlDevTools.prl
%{_libdir}/pkgconfig/Qt5QmlDevTools.pc
%{_datadir}/qt5/mkspecs/modules/qt_lib_qmldevtools_private.pri

