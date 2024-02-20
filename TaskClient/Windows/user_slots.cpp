#include "userwindow.h"
#include "ui_userwindow.h"

// Реакция на клик по строке в таблице с задачами.
void UserWindow::task_clicked(const QModelIndex& index)
{
    if (index.row() == 0) {
        ui->leTaskName->clear();
        ui->leDeadLine->clear();
        ui->leDescription->clear();
        ui->leFullname->clear();
        ui->tvTasks->clearSelection();
        return;
    }

    if (data_keeper_ptr->tasks_is_empty()) {
        return;
    }

    bool ok = false;
    const int task_id = tasks_table_model->item(index.row(), 0)->data(Qt::DisplayRole).toInt(&ok);

    // Проверка определённого значения.
    if ( !ok || task_id <= 0) {
        show_message(QString("Unexpected id error!\nid: %1").arg(QString::number(task_id)));
        ui->tvTasks->clearSelection();
        return;
    }

    ui->leTaskName->setText( data_keeper_ptr->get_task_data(task_id)->name );
    ui->leDeadLine->setText( data_keeper_ptr->get_task_data(task_id)->deadline );
    ui->leDescription->setText( data_keeper_ptr->get_task_data(task_id)->description );

    ui->cbTaskStatus->setCurrentIndex(data_keeper_ptr->get_task_data(task_id)->status - 1);

    const int user_id = data_keeper_ptr->get_task_user(task_id);

    if (user_id <= 0) {
        ui->leFullname->clear();
        return;
    }

    if (!data_keeper_ptr->users_containes(user_id)) {
        return;
    }

    const auto iter = data_keeper_ptr->get_user_data(user_id);

    ui->leFullname->setText(QString("%1 %2 %3")
                            .arg(iter->fullname.second, iter->fullname.first, iter->fullname.patronymic));
}

// Запрос списка задач.
void UserWindow::get_tasks_list()
{
    // Есть ли связь с сервером.
    if (!request_manager_ptr->connected_to_server()) {
        show_message(QString("No connection with server!"));
        return;
    }

    //Блокируем кнопки
    lock_buttons();

    // Запрос списка id задач.
    bool result = (ui->checkbOnlyOwn->isChecked()
                       ? handler_ptr->get_taskslist(data_keeper_ptr->get_own_id())
                       : handler_ptr->get_taskslist());

    if (!result) {
        show_message(QString("Unable to get tasks list\n%1").arg(handler_ptr->get_error()));
        unlock_buttons();
        return;
    }

    // Очищаем коллекцию с данными задач и коллекцию с данными пользователей.
    data_keeper_ptr->tasks_clear();
    data_keeper_ptr->users_clear();

    int user_id = 0;

    // Для всех полученных id запрашиваем данные задачи и данные исполнителя.
    for(auto iter = collector_ptr->task_ids_cib(); iter != collector_ptr->task_ids_cie(); ++iter) {
        // Запрос данных задачи.
        if (!handler_ptr->get_taskdata(*iter)) {
            show_message(QString("Unable to get taskdata\n%1").arg(handler_ptr->get_error()));
            unlock_buttons();
            return;
        }

        // Добавляем данные задачи в коллекцию.
        data_keeper_ptr->add_task( *iter, collector_ptr->get_task_data());

        // id исполнителя задачи.
        user_id = data_keeper_ptr->get_task_user(*iter);

        // Проверяем, были ли данные этого пользователя получены ранее.
        if ( data_keeper_ptr->users_containes(user_id) || (user_id <= 0)) {
            continue;
        }

        // Для каждой задачи запрашиваем данные пользователя - исполнителя.
        // Запрос логина и типа пользователя по user id.
        if (!handler_ptr->get_username(user_id)) {
            show_message(QString("Unable to get login and user type\n%1").arg(handler_ptr->get_error()));
            unlock_buttons();
            return;
        }

        // Запрос ФИО пользователя.
        if (!handler_ptr->get_fullname(user_id)) {
            show_message(QString("Unable to get user fullname\n%1").arg(handler_ptr->get_error()));
            unlock_buttons();
            return;
        }

        // Добавляем данные пользователя в коллекцию.
        data_keeper_ptr->add_user( user_id, UserData(collector_ptr->get_login_and_type(), collector_ptr->get_fullname()));
    }

    int row_number;

    // Очищаем таблицу от предыдущих строк.
    tasks_table_model->removeRows(1, tasks_table_model->rowCount()-1);

    // Вывод данных в таблицу.
    for(auto iter = data_keeper_ptr->tasks_data_cib(); iter != data_keeper_ptr->tasks_data_cie(); ++iter) {
        tasks_table_model->insertRow(tasks_table_model->rowCount());
        row_number = tasks_table_model->rowCount()-1;

        tasks_table_model->setData(tasks_table_model->index(row_number, 0), iter.key(), Qt::DisplayRole);
        tasks_table_model->setData(tasks_table_model->index(row_number, 1), collector_ptr->status_description(iter->status), Qt::DisplayRole);
        tasks_table_model->setData(tasks_table_model->index(row_number, 2), iter->name, Qt::DisplayRole);
        tasks_table_model->setData(tasks_table_model->index(row_number, 3), iter->deadline, Qt::DisplayRole);
        tasks_table_model->setData(tasks_table_model->index(row_number, 4), iter->description, Qt::DisplayRole);
        tasks_table_model->setData(tasks_table_model->index(row_number, 5), QString::number(iter->user_id), Qt::DisplayRole);

        tasks_table_model->setData(tasks_table_model->index(row_number, 6),
                data_keeper_ptr->users_containes(iter->user_id)
                ? (data_keeper_ptr->get_user_data(iter->user_id))->login_type.user_name
                : ((iter->user_id == data_keeper_ptr->get_own_id()) ? data_keeper_ptr->get_own_login() : QString("Unknown")),
                Qt::DisplayRole);
    }

    ui->tvTasks->clearSelection();
    unlock_buttons();
}

// Создать новую или изменить данные выбранной задачи.
void UserWindow::add_or_edit_task()
{
    // Есть ли связь с сервером.
    if (!request_manager_ptr->connected_to_server()) {
        show_message(QString("No connection with server!"));
        return;
    }

    // Создать новую задачу.
    if (ui->rbCreateTask->isChecked()) {
        create_task();
        return;
    }

    // Удалить выбранную задачу.
    if (ui->rbTakeTask->isChecked()) {
        take_task();
        return;
    }

    // Изменить статус задачи.
    if (ui->rbChangeStatus->isChecked()) {
        change_task_status();
        return;
    }

    // Задать новую дату, до которой д/б выполнена задача.
    if (ui->rbSetDeadline->isChecked()) {
        set_task_deadline();
        return;
    }
    
    show_message(QString("Choose an action"));
}

// Очистить поля ввода.
void UserWindow::clear_fields()
{
    ui->leTaskName->clear();
    ui->leDeadLine->clear();
    ui->leDescription->clear();
    ui->leFullname->clear();

    ui->tvTasks->clearSelection();
}
