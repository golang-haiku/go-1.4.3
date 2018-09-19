// Created by cgo -godefs - DO NOT EDIT
// cgo -godefs types_haiku.go

package syscall

const (
	sizeofPtr      = 0x8
	sizeofShort    = 0x2
	sizeofInt      = 0x4
	sizeofLong     = 0x8
	sizeofLongLong = 0x8
)

type (
	_C_short     int16
	_C_int       int32
	_C_long      int64
	_C_long_long int64
)

type Timespec struct {
	Sec       int32
	Pad_cgo_0 [4]byte
	Nsec      int64
}

type Timeval struct {
	Sec  int32
	Usec int32
}

type Rusage struct {
	Utime Timeval
	Stime Timeval
}

type Rlimit struct {
	Cur uint64
	Max uint64
}

type _Gid_t uint32

type Stat_t struct {
	Dev       int32
	Pad_cgo_0 [4]byte
	Ino       int64
	Mode      uint32
	Nlink     int32
	Uid       uint32
	Gid       uint32
	Size      int64
	Rdev      int32
	Blksize   int32
	Atim      Timespec
	Mtim      Timespec
	Ctim      Timespec
	Crtim     Timespec
	Type      uint32
	Pad_cgo_1 [4]byte
	Blocks    int64
}

type Flock_t struct {
	Type      int16
	Whence    int16
	Pad_cgo_0 [4]byte
	Start     int64
	Len       int64
	Pid       int32
	Pad_cgo_1 [4]byte
}

type Dirent struct {
	Dev       int32
	Pdev      int32
	Ino       int64
	Pino      int64
	Reclen    uint16
	Name      [1 + 256]int8
	Pad_cgo_0 [1]byte
}

type RawSockaddrInet4 struct {
	Len    uint8
	Family uint8
	Port   uint16
	Addr   [4]byte /* in_addr */
	Zero   [24]int8
}

type RawSockaddrInet6 struct {
	Len      uint8
	Family   uint8
	Port     uint16
	Flowinfo uint32
	Addr     [16]byte /* in6_addr */
	Scope_id uint32
}

type RawSockaddrUnix struct {
	Len    uint8
	Family uint8
	Path   [126]int8
}

type RawSockaddrDatalink struct {
	Len    uint8
	Family uint8
	E_type uint16
	Index  uint32
	Type   uint8
	Nlen   uint8
	Alen   uint8
	Slen   uint8
	Data   [20]uint8
}

type RawSockaddr struct {
	Len    uint8
	Family uint8
	Data   [30]uint8
}

type RawSockaddrAny struct {
	Addr RawSockaddr
	Pad  [96]int8
}

type _Socklen uint32

type Linger struct {
	Onoff  int32
	Linger int32
}

type Iovec struct {
	Base *byte
	Len  uint64
}

type IPMreq struct {
	Multiaddr [4]byte /* in_addr */
	Interface [4]byte /* in_addr */
}

type IPv6Mreq struct {
	Multiaddr [16]byte /* in6_addr */
	Interface uint32
}

type Msghdr struct {
	Name       *byte
	Namelen    uint32
	Pad_cgo_0  [4]byte
	Iov        *Iovec
	Iovlen     int32
	Pad_cgo_1  [4]byte
	Control    *byte
	Controllen uint32
	Flags      int32
}

type Cmsghdr struct {
	Len   uint32
	Level int32
	Type  int32
}
type IPv6MTUInfo struct {
	Addr RawSockaddrInet6
	Mtu  uint32
}

type Inet6Pktinfo struct {
	Addr    [16]byte /* in6_addr */
	Ifindex uint32
}

type ICMPv6Filter struct {
	Filt [8]uint32
}

const (
	SizeofSockaddrInet4    = 0x20
	SizeofSockaddrInet6    = 0x1c
	SizeofSockaddrAny      = 0x80
	SizeofSockaddrUnix     = 0x80
	SizeofSockaddrDatalink = 0x20
	SizeofLinger           = 0x8
	SizeofIPMreq           = 0x8
	SizeofIPv6Mreq         = 0x14
	SizeofMsghdr           = 0x30
	SizeofCmsghdr          = 0xc
	SizeofIPv6MTUInfo      = 0x20
	SizeofInet6Pktinfo     = 0x14
	SizeofICMPv6Filter     = 0x20
)

type FdSet struct {
	Bits [32]uint32
}

type Termios struct {
	Iflag     uint32
	Oflag     uint32
	Cflag     uint32
	Lflag     uint32
	Line      int8
	Ispeed    uint8
	Ospeed    uint8
	Cc        [11]uint8
	Pad_cgo_0 [2]byte
}

const (
	F_DUPFD_CLOEXEC = 0x0200
)
