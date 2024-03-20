#include "byte_stream.hh"
#include <cstdint>

using namespace std;

ByteStream::ByteStream( uint64_t capacity ) : capacity_( capacity ), buffer_( queue<string>() ) {}

bool Writer::is_closed() const
{
  return closed_;
}

void Writer::push( string data )
{
  if ( closed_ || data.size() == 0 || capacity_ == 0 )
    return;
  if ( data.size() > capacity_ ) {
    data = data.substr( 0, capacity_ );
  }

  buffer_.push( data );
  bytes_pushed_ += data.size();
  capacity_ -= data.size();
}

void Writer::close()
{
  closed_ = true;
}

uint64_t Writer::available_capacity() const
{
  return capacity_;
}

uint64_t Writer::bytes_pushed() const
{
  return bytes_pushed_;
}

bool Reader::is_finished() const
{
  return closed_ && buffer_.empty();
}

uint64_t Reader::bytes_popped() const
{
  return bytes_popped_;
}

string_view Reader::peek() const
{
  if ( bytes_buffered() != 0 )
    return buffer_.front();
  else
    return string_view();
}

void Reader::pop( uint64_t len )
{
  while ( bytes_buffered() > 0 && len > 0 ) {
    string& front = buffer_.front();
    uint64_t bytes_to_pop;
    if ( front.size() <= len ) {
      bytes_to_pop = front.size();
      buffer_.pop();
    } else {
      bytes_to_pop = len;
      front.erase( 0, len );
    }
    bytes_popped_ += bytes_to_pop;
    capacity_ += bytes_to_pop;
    len -= bytes_to_pop;
  }
}

uint64_t Reader::bytes_buffered() const
{
  return bytes_pushed_ - bytes_popped_;
}
