Summary: Demonstrate loading shared library with dlopen 
Name: my_dlopen_so
Version: 1.0.0
Release: demo
License: None
Group: Applications/System
URL: http://github.com/jvkcode/MySvcs/
Source0: http://www.github.com/jvkcode/%{name}-%{version}.tar.gz
Source1: README.%{name}
Buildroot: %{_tmppath}/%{name}-%{version}-%{release}-root-%(%{__id_u} -n)

## Requires:

## BuildRequires: gcc automake autoreconf aclocal libtool m4

## List patches here
## Patch1: 

%description
Run my_dlopen from command line as /relative/or/path/%{name}
If shared library is found in the same directory as %{name}
Outout:
Constructor: done it is work!
my_dlopen_libs_info: Running binaries are 64 bits
my_dlopen_libs_info: Shoud I keep/check libs version here?
Destructor.
Or you see all kinds of errors like:
Constructor: Error on ./libmy_dlopen_so.so.4: 
cannot open shared object file: No such file or directory
Library must be in the same directory as executable
main: Fails to load shared library
Destructor.

%package        devel
Summary:        has .a .la files, source code and junk
Group:          Development/Libraries
Requires:       %{name} = %{version}-%{release}

%description devel
has .a .la files, source code and docs installed

%prep
%setup -n %{name}-%{version}
cp -f %{SOURCE1} README
## Place all paches here
## %patch1 -p1 -b .strip

%build

autoreconf -I build-aux -v
# to build on x86_64 for i386 as:
# rpmbuild --target=i386 -ba my_dlopen_so.spec
%ifarch i386
build_on_host=`uname -m`
if test "${build_on_host}" = "x86_64" ; then
arch_i386_options='--build=i386-linux-gnu --target=i386-linux-gnu --host=x86_64-linux-gnu'
fi
%endif

./configure --libdir=/usr/bin --bindir=/usr/bin ${arch_i386_options}
make

%install
rm -rf $RPM_BUILD_ROOT
make install DESTDIR=$RPM_BUILD_ROOT
mkdir -p $RPM_BUILD_ROOT%{_libdir}
mkdir -p $RPM_BUILD_ROOT/etc/ld.so.conf.d
mv $RPM_BUILD_ROOT%{_bindir}/*.a  $RPM_BUILD_ROOT%{_libdir}
mv $RPM_BUILD_ROOT%{_bindir}/*.la  $RPM_BUILD_ROOT%{_libdir}

echo '# not using ldconfig'> $RPM_BUILD_ROOT/etc/ld.so.conf.d/%{name}.conf

%clean 
rm -rf $RPM_BUILD_ROOT

%files
%defattr(-,root,root)
%attr(0440,root,root) %config(noreplace) /etc/ld.so.conf.d/%{name}.conf
%attr(4111,root,root) %{_bindir}/*

%files devel
%doc README AUTHORS ChangeLog INSTALL NEWS
%attr(0755,root,root) %{_libdir}/*.a
%attr(0755,root,root) %{_libdir}/*.la

