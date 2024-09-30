use std::cmp::Ordering;
use std::cmp::{max, min};
use std::collections::{HashMap, HashSet};
use std::env;
use std::error::Error;
use std::hash::Hash;
use std::io::prelude::*;
use std::io::Write;
use std::iter::*;
use std::str::FromStr;


fn main() {
    let y = Box::new(20);
    let ptr_y = &*y as *const i32;
}
