/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package org.apache.arrow.vector;

/**
 * Interface for all int type vectors.
 */
<<<<<<< HEAD
<<<<<<< HEAD
public interface BaseIntVector extends FieldVector {

  /**
   * Sets the value at index, note this value may need to be need truncated.
   * Note this is safe version (i.e. call setSafe method in vector)
   */
  void setWithPossibleTruncate(int index, long value);

  /**
   * Sets the value at index, note this value may need to be need truncated.
   * Note this is unsafe version (i.e. call set method in vector)
   */
  void setUnsafeWithPossibleTruncate(int index, long value);

  /**
   * Gets the value at index.
   * This value may have been extended to long and will throw {@link NullPointerException}
   * if the value is null. Note null check could be turned off via {@link NullCheckingForGet}.
   */
  long getValueAsLong(int index);
=======
=======
>>>>>>> 106ca580414f7d55261394f0155476baa894f98a
public interface BaseIntVector extends ValueVector {

  /**
   * set the encoded value from a {@link org.apache.arrow.vector.dictionary.Dictionary}.
   */
  void setEncodedValue(int index, int value);
<<<<<<< HEAD
>>>>>>> 5588-Better-support-for-building-UnionArrays
=======
>>>>>>> 106ca580414f7d55261394f0155476baa894f98a
}
