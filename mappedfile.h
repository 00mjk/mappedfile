/* mappedfile.h -- provides a class for memory mapped files
 *
 * Copyright (C) 2008 Benjamin Kramer
 *
 * This software is provided 'as-is', without any express or implied
 * warranty.  In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 *
 */

#ifndef MAPPEDFILE_H
#define MAPPEDFILE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>

char *map_file(const char* path, size_t* length);
void unmap_file(char* data, size_t size);

#ifdef __cplusplus
}

#include <cstddef>
#ifndef NO_EXCEPTIONS
#include <stdexcept>
#else
#include <cstdlib>
#include <iostream>
#endif

/*!
 * MappedFile allows you to create a simple read-only file mapping in an
 * object-oriented cross-platform way.
 */
class MappedFile {
private:
	std::size_t size_;
	char *data_;
	MappedFile(const MappedFile&) {}
	MappedFile& operator=(const MappedFile&) { return *this; }
public:
	/*!
	 * Maps the named file into memory.
	 * The file is mapped into memory. All filehandles are closed afterwards.
	 * \param path path of the file being mapped
	 * \exception IOException the file couldn't be opened
	 */
	MappedFile(const char *path) {
		data_ = map_file(path, &size_);
		if (data_ == NULL) {
#ifndef NO_EXCEPTIONS
			throw IOException(std::string("Couldn't open File \"") + path + "\"");
#else
			std::clog << "Couldn't open File \"" << path << "\"" << std::endl;
			std::exit(EXIT_FAILURE);
#endif
		}
	}
	/*!
	 * Unmaps the file and releases all memory.
	 */
	~MappedFile() {
		unmap_file(data_, size_);
	}
	/*!
	 * Get the size of the file in memory.
	 */
	std::size_t size() const { return size_; }
	/*!
	 * Gets the nth byte from the mapped file.
	 */
	char operator[](std::size_t n) const { return data_[n]; }
	/*!
	 * Gets a read-only pointer to the mapped data.
	 */
	const char* ptr() const { return data_; }

#ifndef NO_EXCEPTIONS
	struct IOException : public std::runtime_error {
		IOException(const std::string& message) : std::runtime_error(message) {}
	};
#endif /* NO_EXCEPTIONS */
};
#endif /* __cplusplus */

#endif /* MAPPEDFILE_H */
