// ---------- string_format ----------

template<typename... Args>
std::string string_format(const char* fmt, Args... args)
{
	size_t size = _snprintf(nullptr, 0, fmt, args...);
	std::string buf;
	buf.reserve(size + 1);
	buf.resize(size);
	_snprintf(&buf[0], size + 1, fmt, args...);
	return buf;
}

