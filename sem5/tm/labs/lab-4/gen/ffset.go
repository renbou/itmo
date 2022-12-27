package gen

// ffset is a helper for first/follow set
type ffset map[int]struct{}

// contains returns true if the ffset contains the specified ID
func (s ffset) contains(id int) bool {
	_, ok := s[id]
	return ok
}

// adds the ID to the set and returns true if the set has changed
func (s ffset) add(id int) bool {
	if _, ok := s[id]; ok {
		return false
	}

	s[id] = struct{}{}
	return true
}

// uniteWith unites the other set with this one and returns true if this set has changed
func (s ffset) uniteWith(s2 ffset) bool {
	var changed bool
	for id := range s2 {
		changed = s.add(id) || changed
	}
	return changed
}

// containsE returns true if the ffset contains the relevant "E" (eps/eof)
func (s ffset) containsE() bool {
	return s.contains(eofTokenID)
}

// withoutE returns this set without the E, even if it contains one
func (s ffset) withoutE() ffset {
	w := make(ffset, len(s))
	for id := range s {
		if id != eofTokenID {
			w[id] = struct{}{}
		}
	}
	return w
}
