// Copyright 2009 The Go Authors. All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

package os

import "syscall"
import "unsafe"

func clen(n []byte) int {
	for i := 0; i < len(n); i++ {
		if n[i] == 0 {
			return i
		}
	}
	return len(n)
}

func (file *File) readdirnames(n int) (names []string, err error) {
	size := n
	if size <= 0 {
		size = 100
		n = -1
	}

	names = make([]string, 0, size) // Empty with room to grow.

	dirfd, err := syscall.Dup(file.fd)
	if err != nil {
		return names, err
	}
	dir, err := syscall.Fdopendir(dirfd)
	if err != nil {
		return names, err
	}
	defer syscall.Closedir(dir)
	var result *syscall.Dirent = nil
	entry := syscall.Dirent{}
	for n != 0 {
		status := syscall.Readdir_r(dir, &entry, &result)
		if status != 0 {
			return names, syscall.Errno(status)
		}
		if result == nil {
			break
		}
		if result.Ino == 0 { // File absent in directory
			continue
		}
		bytes := (*[10000]byte)(unsafe.Pointer(&result.Name[0]))
		name := string(bytes[0:clen(bytes[:])])
		if name == "." || name == ".." {
			// considered "Useless names" by ParseDirent
			continue
		}
		names = append(names, name)
		n--
	}
	return names, err
}
