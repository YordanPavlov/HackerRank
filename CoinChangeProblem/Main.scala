
object Result {

  /*
   * Complete the 'getWays' function below.
   *
   * The function is expected to return a LONG_INTEGER.
   * The function accepts following parameters:
   *  1. INTEGER n
   *  2. LONG_INTEGER_ARRAY c
   */

  def tryIncrementBuffer(buffer: Array[Long], index: Int, value: Long): Unit = {
    if (index < buffer.length) {
      buffer(index) = buffer(index) + value
    }
  }
  def getWays(n: Int, c: Array[Long]): Long = {
    val buffer = new Array[Long](n+1)

    c.foreach(coin => {
      tryIncrementBuffer(buffer, coin.toInt, 1)
      for (i <- 0 to n) {
        if (buffer(i) > 0) {
          tryIncrementBuffer(buffer, i + coin.toInt, buffer(i))
        }
      }
    })

    buffer(n)
  }

}


object Main  {
  def main(args: Array[String]) {
    val input = Array(2L, 5L, 3L, 6L)
    val result = Result.getWays(10, input)
    println(result)
  }
}
