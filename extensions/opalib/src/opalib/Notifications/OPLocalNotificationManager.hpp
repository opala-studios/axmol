//
// Created by Opala Studios on 03/09/21.
//

#ifndef PROJ_ANDROID_OPLOCALNOTIFICATIONMANAGER_HPP
#define PROJ_ANDROID_OPLOCALNOTIFICATIONMANAGER_HPP

#include <string>
#include <memory>
#include <cocos2d.h>

/**
 * Responsável em agendar a notificações locais do app.
 * Utiliza o alarme do dispositivo para agendar e mostrar as notificações no tempo determinado.
 *
 * Obs: Na parte do android, antes de Utilizar certifique-se que configurar o android manifest,
 *     e os arquivos de strings.
 */
class OPLocalNotificationManager
{
public:
    virtual ~OPLocalNotificationManager() {}

    static std::shared_ptr<OPLocalNotificationManager> shared();

    OPLocalNotificationManager& setType(const std::string& type);
    
    /**
     * Inicializa o gerenciador e os componentes nativos necessários.
     * Obs: encapsula o método setup.
     */
    void initialize();

    /**
     * Para configurar os componentes nativos
     */
    virtual void setup();

    /**
     * Agenda uma notificação utilizando o tempo atual do sistema no momento da chamada do método (pois a notificação é via alarme).
     * O usuário será notificado na HORA AGENDADA mais o intervalo de repetição (timeRepeat).
     *
     * Exemplo:
     *      Se a chamado do método for as 8:00 e o tempo de intervalo 24 horas,
     *      o usuário será notifica as 8:00 do dia seguinte.
     *
     * @code
     *  // o tempo é em segundos
     *  int time = 24 * SECS_HOURS;
     *
     *  OPLocalNotificationManager::shared()->scheduleNotification(
     *      "IT_NOTIFICATION_OF_USE_" + std::to_string(i + 1), // chave da notificação
     *      title, // titulo
     *      message, // mensagem
     *      time // tempo em segundos
     *  );
     * @endCode
     *
     * @param actionName (std::string) - nome da ação do alarme para agendar a notificação na parte nativa,
     *                            e usar como chave no json  de alarmes.
     * @param title  (std::string)
     * @param message  (std::string)
     * @param timeRepeat  (int) - intervalo de disparo e tempo de repetição
     * @param repeat  (bool) - para identificar se irá disparar a cada intervalo de repetição (se ativo, vai repetir sempre).
     */
    virtual void scheduleNotification(const std::string& actionName,
                                      const std::string& title,
                                      const std::string& message,
                                      const int timeRepeat = 0,
                                      const bool repeat = false);
    
    /**
     * Agenda uma notificação em uma determinada hora e minuto informada como parâmetro.
     *
     * Exemplo:
     *      Se informar no parâmetro as 8:15, o usuário será notifica as 8:15 do dia seguinte.
     *
     * @code
     *  OPLocalNotificationManager::shared()->scheduleNotification(
     *      "IT_NOTIFICATION_OF_USE_" + std::to_string(i + 1), // chave da notificação
     *      title, // titulo
     *      message, // mensagem
     *      8, 15, // hora e minuto, que deseja mostrar a notificação
     *  );
     * @endCode
     *
     * @param actionName (std::string) - nome da ação do alarme para agendar a notificação na parte nativa,
     *                            e usar como chave no json  de alarmes.
     * @param title  (std::string)
     * @param message  (std::string)
     * @param hours  (int) - hora dos disparo
     * @param minutes  (int) - minuto do disparo
     * @param repeat  (bool) - para identificar se irá disparar todos os dias no mesmo horário.
     */
    virtual void scheduleNotification(const std::string& actionName,
                                      const std::string& title,
                                      const std::string& message,
                                      const int hours,
                                      const int minutes,
                                      const bool repeat = false);

    /**
     * Remove uma notificação agendada.
     *
     * @param actionName (std::string) - nome da ação do alarme da notificação agendada na parte nativa,
     *                            e chave no json  de alarmes, para desagendar e remover do json.
     */
    virtual void unscheduleNotification(const std::string& actionName);
    
    /**
     * Pra fechar todas as notificação do app abertas
     */
    virtual void closeAllNotifications() {}

protected:
    OPLocalNotificationManager();
    
    virtual void setupScheduleNotification(const std::string& actionName, const std::string& title,
                                           const std::string& message, const int hours,
                                           const int minutes, const int timeRepeat,
                                           const bool repeat);

    struct TimeInfo { int hour; int minute; };
    TimeInfo checkTime(const int hours, const int minutes) const;

    int checkTimeRepeat(const int timeRepeat) const;

protected:
    std::shared_ptr<OPLocalNotificationManager> _shared;
    std::string _type;
};

#endif //PROJ_ANDROID_OPLOCALNOTIFICATIONMANAGER_HPP
