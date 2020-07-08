/*The MIT License (MIT)

Copyright (c) <year> <copyright holders>

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.*/
#ifndef JSON_WRITER_H_INCLUDED
# define JSON_WRITER_H_INCLUDED

# include "value.h"
# include <vector>
# include <string>
# include <iostream>

namespace Json {

   class Value;

   /** \brief Abstract class for writers.
    */
   class JSON_API Writer
   {
   public:
      inline virtual ~Writer();

      inline virtual std::string write( const Value &root ) = 0;
   };

   /** \brief Outputs a Value in <a HREF="http://www.json.org">JSON</a> format without formatting (not human friendly).
    *
    * The JSON document is written in a single line. It is not intended for 'human' consumption,
    * but may be usefull to support feature such as RPC where bandwith is limited.
    * \sa Reader, Value
    */
   class JSON_API FastWriter : public Writer
   {
   public:
      inline FastWriter();
      inline virtual ~FastWriter(){}

      inline void enableYAMLCompatibility();

   public: // overridden from Writer
      inline virtual std::string write( const Value &root );

   private:
      inline void writeValue( const Value &value );

      std::string document_;
      bool yamlCompatiblityEnabled_;
   };

   /** \brief Writes a Value in <a HREF="http://www.json.org">JSON</a> format in a human friendly way.
    *
    * The rules for line break and indent are as follow:
    * - Object value:
    *     - if empty then print {} without indent and line break
    *     - if not empty the print '{', line break & indent, print one value per line
    *       and then unindent and line break and print '}'.
    * - Array value:
    *     - if empty then print [] without indent and line break
    *     - if the array contains no object value, empty array or some other value types,
    *       and all the values fit on one lines, then print the array on a single line.
    *     - otherwise, it the values do not fit on one line, or the array contains
    *       object or non empty array, then print one value per line.
    *
    * If the Value have comments then they are outputed according to their #CommentPlacement.
    *
    * \sa Reader, Value, Value::setComment()
    */
   class JSON_API StyledWriter: public Writer
   {
   public:
      inline StyledWriter();
      inline virtual ~StyledWriter(){}

   public: // overridden from Writer
      /** \brief Serialize a Value in <a HREF="http://www.json.org">JSON</a> format.
       * \param root Value to serialize.
       * \return String containing the JSON document that represents the root value.
       */
      inline virtual std::string write( const Value &root );

   private:
      inline void writeValue( const Value &value );
      inline void writeArrayValue( const Value &value );
      inline bool isMultineArray( const Value &value );
      inline void pushValue( const std::string &value );
      inline void writeIndent();
      inline void writeWithIndent( const std::string &value );
      inline void indent();
      inline void unindent();
      inline void writeCommentBeforeValue( const Value &root );
      inline void writeCommentAfterValueOnSameLine( const Value &root );
      inline bool hasCommentForValue( const Value &value );
      inline static std::string normalizeEOL( const std::string &text );

      typedef std::vector<std::string> ChildValues;

      ChildValues childValues_;
      std::string document_;
      std::string indentString_;
      int rightMargin_;
      int indentSize_;
      bool addChildValues_;
   };

   /** \brief Writes a Value in <a HREF="http://www.json.org">JSON</a> format in a human friendly way,
        to a stream rather than to a string.
    *
    * The rules for line break and indent are as follow:
    * - Object value:
    *     - if empty then print {} without indent and line break
    *     - if not empty the print '{', line break & indent, print one value per line
    *       and then unindent and line break and print '}'.
    * - Array value:
    *     - if empty then print [] without indent and line break
    *     - if the array contains no object value, empty array or some other value types,
    *       and all the values fit on one lines, then print the array on a single line.
    *     - otherwise, it the values do not fit on one line, or the array contains
    *       object or non empty array, then print one value per line.
    *
    * If the Value have comments then they are outputed according to their #CommentPlacement.
    *
    * \param indentation Each level will be indented by this amount extra.
    * \sa Reader, Value, Value::setComment()
    */
   class JSON_API StyledStreamWriter
   {
   public:
      inline StyledStreamWriter( std::string indentation="\t" );
      inline ~StyledStreamWriter(){}

   public:
      /** \brief Serialize a Value in <a HREF="http://www.json.org">JSON</a> format.
       * \param out Stream to write to. (Can be ostringstream, e.g.)
       * \param root Value to serialize.
       * \note There is no point in deriving from Writer, since write() should not return a value.
       */
      inline void write( std::ostream &out, const Value &root );

   private:
      inline void writeValue( const Value &value );
      inline void writeArrayValue( const Value &value );
      inline bool isMultineArray( const Value &value );
      inline void pushValue( const std::string &value );
      inline void writeIndent();
      inline void writeWithIndent( const std::string &value );
      inline void indent();
      inline void unindent();
      inline void writeCommentBeforeValue( const Value &root );
      inline void writeCommentAfterValueOnSameLine( const Value &root );
      inline bool hasCommentForValue( const Value &value );
      inline static std::string normalizeEOL( const std::string &text );

      typedef std::vector<std::string> ChildValues;

      ChildValues childValues_;
      std::ostream* document_;
      std::string indentString_;
      int rightMargin_;
      std::string indentation_;
      bool addChildValues_;
   };

   inline std::string JSON_API valueToString( Int value );
   inline std::string JSON_API valueToString( UInt value );
   inline std::string JSON_API valueToString( double value );
   inline std::string JSON_API valueToString( bool value );
   inline std::string JSON_API valueToQuotedString( const char *value );

   /// \brief Output using the StyledStreamWriter.
   /// \see Json::operator>>()
   inline std::ostream& operator<<( std::ostream&, const Value &root );

} // namespace Json

#include "json_writer.inl"

#endif // JSON_WRITER_H_INCLUDED
