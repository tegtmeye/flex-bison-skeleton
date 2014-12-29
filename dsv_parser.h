/*
Copyright (c) 2014, Mike Tegtmeyer
All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
this list of conditions and the following disclaimer in the documentation
and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors
may be used to endorse or promote products derived from this software without
specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef LIBDSV_PARSER_H
#define LIBDSV_PARSER_H

#include <unistd.h>

#if defined(__cplusplus)
extern "C" {
#endif

  /**
   * \file dsv_parser.h
   * \brief The main header file for the Delimited Seperated Values library
   *
   *
   */

  /**
   *  \brief A structure containing a dsv parser
   */
  typedef struct {
    void *p;
  } dsv_parser_t;

  /**
   *  \brief Initialize a dsv_parser_t object.
   *
   *  \note You must eventually call dsv_parser_destroy.
   *
   *  \retval 0 success
   *  \retval ENOMEM Could not allocate memory
   */
  int dsv_parser_create(dsv_parser_t *parser);

  /**
   *  \brief Destroy the obj_parser_t object.
   *
   *  \note
   *  Use of any other function except \c dsv_parser_create with \c parser is
   *  undefined.
   */
  void dsv_parser_destroy(dsv_parser_t parser);



  /**
   *  A structure containing the dsv callbacks
   */
  typedef struct {
    void *p;
  } dsv_operations_t;

  /**
   *  \brief Initialize a dsv_operations_t object.
   *
   *  \note You must eventually call dsv_parser_destroy.
   *
   *  \retval 0 success
   *  \retval ENOMEM Could not allocate memory
   */
  int dsv_operations_create(dsv_operations_t *operations);

  /**
   *  \brief Destroy the dsv_operations_t object.
   *
   *  \note
   *  Use of any other function except \c dsv_operations_create with
   *  \c operations is undefined.
   */
  void dsv_operations_destroy(dsv_operations_t operations);

  /**
   *  Callback operatons type definitions
   */

  /**
   *  \brief This function will be called for each record parsed in the file. See
   *  the documentation for the definition of a record.
   *
   *  \param[in] fields \parblock
   *  A c-array of null-terminated byte strings containing each parsed field. If
   *  the field was surrounded by quotes, the string does not inlcude this. No
   *  attempt is made at understanding the content of the string. For example, if
   *  the string contains a number, no attempt is made at determining if the
   *  number will not overflow. Additionally, if quoted properly (see format
   *  documentation), certain non-printing characters may appear in the field
   *  such as newlines. In all cases however, since delimited seperated values
   *  are strictly an ASCII format, no binary shall appear in the field.
   *  \endparblock
   *  \param[in] size The size of the field array
   *  \param[in] context A user-defined value associted with this callback set in ???
   *
   *  \retval nonzero if procssing should continue or 0 if processing should cease
   *          and control should return from the parse function.
   */
  typedef int (*record_callback_t)(const char *fields[], size_t size, void *context);

  /**
   *  \brief Obtain the callback currently set for records
   *
   *  \retval 0 No callback is registered or \c operations has not been initialized
   *  \retval nonzero The currently registered callback
   */
  record_callback_t dsv_get_record_callback(dsv_operations_t operations);

  /**
   *  \brief Obtain the user-defined context currently set for records
   *
   *  \retval 0 No context is registered or \c operations has not been initialized
   *  \retval nonzero The currently registered context
   */
  void * dsv_get_record_context(dsv_operations_t operations);

  /**
   *  \brief Associate the callback \c fn and a user-specified value \c context
   *  with \c operation.
   *
   *  \note The value of \c context is passed in as the \c context parameter in \c fn
   *
   *  \retval 0 on success
   *  \retval EINVAL \c operations has not been initialized
   */
  int dsv_set_record_callback(record_callback_t fn, void *context, dsv_operations_t operations);

  /**
   *  \brief Parse the file \c filename with \c parser, using the operations
   *  contained in \c operations
   *
   *  If the filename begins with the '/' character, then the file is
   *  understood to be an absolute path starting at the root directory. If the
   *  filename does not begin with a '/' character, the file is understood to
   *  be relative to the current working directory. In either case any additional
   *  relative file inclusion mechanism will be relative to the directory that
   *  holds the input file.
   *
   *  If unable to successfully parse 'filename' a negative value will be
   *  returned and the reason can be obtained by \c dsv_parse_error.
   *
   *  Warnings or information about non-implemented features will return success
   *  and be reported via \c dsv_parse_error.
   *
   *  \retval 0 success
   *  \retval ENOMEM out of memory
   *  \retval EINVAL \c parser or \c operations have not been initialized
   *  \retval >0 Any error code returned by fopen
   *  \retval <0 failure, see dsv_parse_error
   */
  int dsv_parse(const char *filename, dsv_parser_t parser,
                dsv_operations_t operations);


  typedef enum {
    dsv_log_none = 0,
    dsv_log_error = 1L,
    dsv_log_warning = 1L << 1,
    dsv_log_info = 1L << 2,
    dsv_log_all = (dsv_log_error|dsv_log_warning|dsv_log_info)
  } dsv_log_level;


  /**
   *  \brief Filter the log messages associated with the last parse operation
   *  according to the given \c log_level and place the first \c len characters into
   *  the buffer pointed to by \c buf returning the total size of the filtered log
   *  messages. Providing a 0 \c buf value is permitted.
   *
   *  To avoid memory ownership and potential buffer overflow issues, the recommended
   *  pattern for use of \c dsv_parse_error is:
   *
   *      // Get the size of the filtered messages
   *      size_t len = dsv_parse_error(aParser,dsv_parse_error,0,0);
   *
   *      // checks for a zero len and errors ommitted
   *
   *      // Allocate buf based on the returned size 'len'
   *      char *buf = (char*)malloc(sizeof(char)*len);
   *
   *      // copy the messages to buf
   *      dsv_parse_error(aParser,dsv_parse_error,&buf,len);
   *
   *  \note All messages are concatenated together
   *
   *  \parap[in] parser A \c dsv_parser_t object previously initialized with
   *                    \c dsv_parser_create
   *  \param[in] log_level A valid value of \c dsv_log_level indicating how the
   *                        the list of all log messages should be filtered.
   *  \param[in,out] buf A pointer to a character buffer appropriately sized. If \c buf
   *                      is nonzero, the first \c len characters of messages are copied
   *                      to buf including the terminating null character
   *  \param[in] len The size of the character buffer pointed to by \c buf.
   *  \retval 0 An error has occurred. Possible reasons could be out of memory, but
   *            more likely is that \c parser was not initialized depending on how
   *            \c parser was declared. That is, using:
   *                dsv_parser_t parser = {}; // will notice the lack of initialization
   *                dsv_parser_t parser; // will likely crash
   *  \retval nonzero The size of the message buffer filtered to \c log_level including
   *                  the null terminator. This value is appropriate for allocating the
   *                  character buffer \c buf in subsequent calls.
   */
  size_t dsv_parse_error(dsv_parser_t parser, dsv_log_level log_level, char *buf, size_t len);



#if defined(__cplusplus)
}
#endif

#endif
