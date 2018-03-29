#ifndef DATA_MESSAGE_H_
#define DATA_MESSAGE_H_

#include <cstddef>
#include <string>
#include <vector>

#include "message.h"
#include "module_info.h"


namespace wrp
{

/** Представляет идентификатор копии комплекта данных вычислительного модуля.
 *
 */
typedef std::size_t TBatchInstanceId;

/** Значение по умолчанию для объектов типа TBatchInstanceId.
 *
 */
const TBatchInstanceId BatchInstanceIdUndefined = 0;

/** Представляет информацию о входе, данные которого присутствуют в сообщении.
 *
 */
struct TDataMessageEntranceInfo
{
  /** Представляет размер данных в сообщении.
   *
   */
  typedef unsigned long int TMessageDataSize;

  /** Смещение передаваемого фрагмента данных.
   *
   */
  TMessageDataSize startOffset;

  /** Общий размер сообщения вычислительного модуля.
   *
   */
  TMessageDataSize totalSize;

  /** Размер передаваемого фрагмента данных.
   *
   */
  TMessageDataSize fragmentSize;

  /** Идентификатор входа, которому соответствует фрагмент данных.
   *
   */
  std::string entranceId;

  /** Фрагмент данных сообщения.
   * Владеет указателем.
   */
  char* data;

  /** Конструктор по умолчанию.
   *
   */
  TDataMessageEntranceInfo();

  /** Конструктор копирования.
   *
   */
  TDataMessageEntranceInfo(const TDataMessageEntranceInfo& other);

  /** Деструктор.
   *
   */
  ~TDataMessageEntranceInfo();
};

/** Представляет метку модуля, через который прошло сообщение.
 *
 */
struct TDataMessageToken
{
  /** Идентификатор вычислительного модуля.
   *
   */
  TModuleId source;

  /** Идентификатор комплекта сформированных данных.
   *
   */
  TBatchInstanceId batchId;

  /** Конструктор по умолчанию.
   *
   */
  TDataMessageToken(const TModuleId& source = TModuleId(), 
    const TBatchInstanceId& batchId = BatchInstanceIdUndefined);

  /** Оператор проверки на равенство.
   *
   */
  bool operator==(const TDataMessageToken& other) const;
  bool operator!=(const TDataMessageToken& other) const;
};

/** Структура с метками модулей, через которые прошли данные.
 *
 */
struct TDataMessageTokens
{
  /** Набор меток, определяющих последовательность прохождения данных
   *  через модули-производители.
   *
   */
  std::vector<TDataMessageToken> tokens;

  /** Метка последнего модуля, через который прошли данные.
   *
   */
  TDataMessageToken lastModuleToken;

  /** Конструктор по умолчанию.
   *
   */
  TDataMessageTokens();

  /** Оператор проверки на равенство.
   *
   */
  bool operator==(const TDataMessageTokens& other) const;
  bool operator!=(const TDataMessageTokens& other) const;
};

/** Представляет сообщение с данными для вычислительного модуля.
 *
 */
struct TDataMessage : public TMessage
{
  /** Набор меток модулей, через которые прошли данные.
   *
   */
  TDataMessageTokens id;

  /** Информация о фрагментах данных сообщений по нескольким входам.
   *
   */
  std::vector<TDataMessageEntranceInfo> entrances;

  /** Конструктор.
   * \param[in] type Тип сообщения
   * \param[in] source Идентификатор модуля-источника сообщения
   * \param[in] destination Идентификатор модуля-получателя сообщения
   */
  TDataMessage(EMessageType::Type type,
    const TModuleId& source = TModuleId(),
    const TModuleId& destination = TModuleId());

  /** Деструктор.
   *
   */
  virtual ~TDataMessage(); /* override */
};

/** Представляет последовательность, созданную генерирующим комплектом.
 *
 */
struct TBatchSequence
{
  /** Идентификатор первого комплекта.
   *
   */
  TBatchInstanceId firstId;

  /** Идентификатор последнего комплекта.
   *
   */
  TBatchInstanceId lastId;

  /** Токены, общие для всей последовательности.
   * В последнем элементе идентификатор комплекта (#TBatchInstanceId)
   * изменяется по ходу последовательности в пределах,
   * указанных в полях firstId и lastId.
   */
  std::vector<TDataMessageToken> tokens;

  /** Идентификаторы выходных каналов генерирующего комплекта.
   *
   */
  std::vector<std::string> sourceChannels;

  TBatchSequence();
  ~TBatchSequence();
};

} // namespace wrp

#endif // DATA_MESSAGE_H_
