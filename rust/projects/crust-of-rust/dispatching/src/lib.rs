struct RcBox<T: ?Sized> {
    value: T,
    ref_count: usize,
}

pub fn strlen(s: impl AsRef<str>) -> usize {
    s.as_ref().len()
}

