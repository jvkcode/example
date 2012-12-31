Summary: Simple Version Control System allows to save all files in order
Name: MySvcs
Version: 1.0.0
Release: 1test
License: None
Group: Applications/System
URL: http://github.com/jvkcode/MySvcs/
Source0: http://www.github.com/jvkcode/%{name}-%{version}.tar.gz
Source1: README.txt
Buildroot: %{_tmppath}/%{name}-%{version}-%{release}-root-%(%{__id_u} -n)
## Requires:

#BuildRequires: gcc-4.7

## Patch1: 

%description
MySvcs is Simple Version Control written in c. It creates
storage for saving text files and gives tools to keep and
retrive old version of your files by revision number or by
tag. It does not allow you to delete files. It saves files
'as is'. To see difference between revisions - run diff by
hand.

%package        libs
Summary:        has libraries for MySvcs package
Group:          Development/Libraries
Requires:       %{name} = %{version}-%{release}

%description libs
has set of .a libs for nothing

%prep
%setup -n %{name}-%{version}
cp %{SOURCE1} `pwd`

## %patch1 -p1 -b .strip

%build
export CFLAGS="$RPM_OPT_FLAGS $F_PIE" LDFLAGS="-pie -Wl,-z,relro -Wl,-z,now"
# make
mkdir obj
for fl in bin objlib lib; do
    rm -rf ${fl}
    mkdir ${fl}
done
lib_files="my_svcs_libcheckin my_svcs_fileslibs my_svcs_utilslibs my_svcs_libcheckout my_svcs_initrepositorylibs my_svcs_cmdlinelibs"
lib_files_o=
for fl in ${lib_files}; do
    gcc ${CFLAFS} -c ${fl}.c -o objlib/${fl}.o
    lib_files_o="${lib_files_o} objlib/${fl}.o"
done;
ar rcs lib/libmysvcs.a ${lib_files_o}
gcc ${CFLAGS} -static my_svcs_main.c -L./lib -lmysvcs -o bin/MySvcs

%install
rm -rf $RPM_BUILD_ROOT
install -p -d -m 755 $RPM_BUILD_ROOT%{_bindir}
install -p -d -m 755 $RPM_BUILD_ROOT%{_libdir}
install -p -d -m 750 $RPM_BUILD_ROOT/etc/MySvcs
cp bin/MySvcs $RPM_BUILD_ROOT/usr/bin
cp lib/libmysvcs.a $RPM_BUILD_ROOT%{_libdir}

%clean 
rm -rf $RPM_BUILD_ROOT

%files
%defattr(-,root,root)
%doc README*
%attr(0440,root,root) %config(noreplace) /etc/MySvcs
%attr(4111,root,root) %{_bindir}/MySvcs

%files libs
%attr(0755,root,root) %{_libdir}/libmysvcs.a

