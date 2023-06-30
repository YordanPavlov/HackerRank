import scala.util.Sorting.quickSort

object Result {
  def equal(arr: Array[Int]): Int = {
    quickSort(arr)
    var countNormal = 0L
    // That's the tricky part. Increasing all differences by 1 or 2 can actually improve the result, becuase
    // if we have a difference of 4, we need two operations to eliminate it, while a difference of 5 can be eliminated
    // with a single operation.
    var countPlusOne = 1L
    var countPlusTwo = 1L

    for(i <- 1 until arr.length) {
      val dif = arr(i) - arr(0)

      countNormal += getWays(dif)
      countPlusOne += getWays(dif + 1)
      countPlusTwo += getWays(dif + 2)
    }

    scala.math.min(scala.math.min(countNormal, countPlusOne), countPlusTwo).toInt
  }


  def getWays(n: Int): Int = {
    n / 5 + (n % 5) / 2 + (n % 5) % 2
  }


}



object Main  {
  def main(args: Array[String]): Unit = {
    val input = Array(512,125,928,381,890,90,512,789,469,473,908,990,195,763,102,643,458,366,684,857,126,534,974,875,459,892,686,373,127,297,576,991,774,856,372,664,946,237,806,767,62,714,758,258,477,860,253,287,579,289,496)

    val result = Result.equal(input)
    println(result)
  }
}

