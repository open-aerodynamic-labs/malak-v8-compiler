/**
 * @author airduck-vincent
 */
package object v8 {
  type List[A] = java.util.List[A]
  type ArrayList[A] = java.util.ArrayList[A]
  type Map[A, B] = java.util.Map[A, B]
  type HashMap[A, B] = java.util.HashMap[A, B]

  def listof[A]() = new ArrayList[A]()
  def mapof[A, B]() = new HashMap[A, B]()

}
