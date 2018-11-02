#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
// Include the gtest header file:
#include <gtest/gtest.h>

// Include assignment header file:
#include "../src/hw.h"

using namespace std;

class ProjectTests : public ::testing::Test
{
protected:
  ProjectTests() {}          //constructor runs before each test
  virtual ~ProjectTests() {} //destructor cleans up after tests
  virtual void SetUp() {}    //sets up before each test (after constructor)
  virtual void TearDown() {} //clean up after each test, (before destructor)
};

// The following definitions are included to test the internal
// implementation of the atm and bank components:

int atm(int bank_out_fd, int atm_in_fd, int atm_id, Command *cmd);
int bank(int atm_out_fd[], Command *cmd, int *atm_cnt);
int *get_accounts();

//////////////////////////////////////////////////////////////////////
///////////// errors unit tests
//////////////////////////////////////////////////////////////////////

TEST(ProjectTests, test_error_msg_SUCCESS)
{
  const char *msg = "this is successful";
  int type = SUCCESS;
  error_msg(type, msg);
  ASSERT_EQ(error_type(), type) << "error_type() should return " << type;
  ASSERT_STREQ(msg, error_msg_str());
}

TEST(ProjectTests, test_error_msg_ERR_UNKNOWN_CMD)
{
  const char *msg = "unknown command error";
  int type = ERR_UNKNOWN_CMD;
  error_msg(type, msg);
  ASSERT_EQ(error_type(), type) << "error_type() should return " << type;
  ASSERT_STREQ(msg, error_msg_str());
}

TEST(ProjectTests, test_error_msg_ERR_PIPE_READ_ERR)
{
  const char *msg = "pipe read error";
  int type = ERR_PIPE_READ_ERR;
  error_msg(type, msg);
  ASSERT_EQ(error_type(), type) << "error_type() should return " << type;
  ASSERT_STREQ(msg, error_msg_str());
}

TEST(ProjectTests, test_error_msg_ERR_PIPE_WRITE_ERR)
{
  const char *msg = "pipe write error";
  int type = ERR_PIPE_WRITE_ERR;
  error_msg(type, msg);
  ASSERT_EQ(error_type(), type) << "error_type() should return " << type;
  ASSERT_STREQ(msg, error_msg_str());
}

TEST(ProjectTests, test_error_msg_ERR_UNKNOWN_ACCOUNT)
{
  const char *msg = "unknown account error";
  int type = ERR_UNKNOWN_ACCOUNT;
  error_msg(type, msg);
  ASSERT_EQ(error_type(), type) << "error_type() should return " << type;
  ASSERT_STREQ(msg, error_msg_str());
}

TEST(ProjectTests, test_error_msg_ERR_UNKNOWN_ATM)
{
  const char *msg = "unknown atm error";
  int type = ERR_UNKNOWN_ATM;
  error_msg(type, msg);
  ASSERT_EQ(error_type(), type) << "error_type() should return " << type;
  ASSERT_STREQ(msg, error_msg_str());
}

TEST(ProjectTests, test_error_msg_ERR_BAD_TRACE_FILE)
{
  const char *msg = "bad trace file error";
  int type = ERR_BAD_TRACE_FILE;
  error_msg(type, msg);
  ASSERT_EQ(error_type(), type) << "error_type() should return " << type;
  ASSERT_STREQ(msg, error_msg_str());
}

//////////////////////////////////////////////////////////////////////
///////////// command unit tests
//////////////////////////////////////////////////////////////////////

TEST(ProjectTests, test_command_ok)
{
  Command cmd;
  cmd_t c;
  int i = 1, f = 2, t = 3, a = 4;
  MSG_OK(&cmd, i, f, t, a);
  cmd_unpack(&cmd, &c, &i, &f, &t, &a);
  ASSERT_EQ(c, OK) << "command should be ok, received " << c;
  ASSERT_EQ(i, 1) << "i should be 1, received " << i;
  ASSERT_EQ(f, 2) << "f should be 2, received " << f;
  ASSERT_EQ(t, 3) << "t should be 3, received " << t;
  ASSERT_EQ(a, 4) << "a should be 4, received " << a;
}

TEST(ProjectTests, test_command_nofunds)
{
  Command cmd;
  cmd_t c;
  int i = 1, f = 2, t = 3, a = 4;
  MSG_NOFUNDS(&cmd, i, f, a);
  cmd_unpack(&cmd, &c, &i, &f, &t, &a);
  ASSERT_EQ(c, NOFUNDS) << "command should be no funds, received " << c;
  ASSERT_EQ(i, 1) << "i should be 1, received " << i;
  ASSERT_EQ(f, 2) << "f should be 2, received " << f;
  ASSERT_EQ(t, -1) << "t should be -1, received " << t;
  ASSERT_EQ(a, 4) << "a should be 4, received " << a;
}

TEST(ProjectTests, test_command_connect)
{
  Command cmd;
  cmd_t c;
  int i = 1, f = 2, t = 3, a = 4;
  MSG_CONNECT(&cmd, i);
  cmd_unpack(&cmd, &c, &i, &f, &t, &a);
  ASSERT_EQ(c, CONNECT) << "command should be connect, received " << c;
  ASSERT_EQ(i, 1) << "i should be 1, received " << i;
  ASSERT_EQ(f, -1) << "f should be -1, received " << f;
  ASSERT_EQ(t, -1) << "t should be -1, received " << t;
  ASSERT_EQ(a, -1) << "a should be -1, received " << a;
}

TEST(ProjectTests, test_command_exit)
{
  Command cmd;
  cmd_t c;
  int i = 1, f = 2, t = 3, a = 4;
  MSG_EXIT(&cmd, i);
  cmd_unpack(&cmd, &c, &i, &f, &t, &a);
  ASSERT_EQ(c, EXIT) << "command should be exit, received " << c;
  ASSERT_EQ(i, 1) << "i should be 1, received " << i;
  ASSERT_EQ(f, -1) << "f should be -1, received " << f;
  ASSERT_EQ(t, -1) << "t should be -1, received " << t;
  ASSERT_EQ(a, -1) << "a should be -1, received " << a;
}

TEST(ProjectTests, test_command_deposit)
{
  Command cmd;
  cmd_t c;
  int i = 1, f = 2, t = 3, a = 4;
  MSG_DEPOSIT(&cmd, i, t, a);
  cmd_unpack(&cmd, &c, &i, &f, &t, &a);
  ASSERT_EQ(c, DEPOSIT) << "command should be deposit, received " << c;
  ASSERT_EQ(i, 1) << "i should be 1, received " << i;
  ASSERT_EQ(f, -1) << "f should be -1, received " << f;
  ASSERT_EQ(t, 3) << "t should be 3, received " << t;
  ASSERT_EQ(a, 4) << "a should be 4, received " << a;
}

TEST(ProjectTests, test_command_withdraw)
{
  Command cmd;
  cmd_t c;
  int i = 1, f = 2, t = 3, a = 4;
  MSG_WITHDRAW(&cmd, i, f, a);
  cmd_unpack(&cmd, &c, &i, &f, &t, &a);
  ASSERT_EQ(c, WITHDRAW) << "command should be withdraw, received " << c;
  ASSERT_EQ(i, 1) << "i should be 1, received " << i;
  ASSERT_EQ(f, 2) << "f should be 2, received " << f;
  ASSERT_EQ(t, -1) << "t should be -1, received " << t;
  ASSERT_EQ(a, 4) << "a should be 4, received " << a;
}

TEST(ProjectTests, test_command_transfer)
{
  Command cmd;
  cmd_t c;
  int i = 1, f = 2, t = 3, a = 4;
  MSG_TRANSFER(&cmd, i, f, t, a);
  cmd_unpack(&cmd, &c, &i, &f, &t, &a);
  ASSERT_EQ(c, TRANSFER) << "command should be transfer, received " << c;
  ASSERT_EQ(i, 1) << "i should be 1, received " << i;
  ASSERT_EQ(f, 2) << "f should be 2, received " << f;
  ASSERT_EQ(t, 3) << "t should be 3, received " << t;
  ASSERT_EQ(a, 4) << "a should be 4, received " << a;
}

TEST(ProjectTests, test_command_balance)
{
  Command cmd;
  cmd_t c;
  int i = 1, f = 2, t = 3, a = 4;
  MSG_BALANCE(&cmd, i, f);
  cmd_unpack(&cmd, &c, &i, &f, &t, &a);
  ASSERT_EQ(c, BALANCE) << "command should be balance, received " << c;
  ASSERT_EQ(i, 1) << "i should be 1, received " << i;
  ASSERT_EQ(f, 2) << "f should be 2, received " << f;
  ASSERT_EQ(t, -1) << "t should be -1, received " << t;
  ASSERT_EQ(a, -1) << "a should be -1, received " << a;
}

//////////////////////////////////////////////////////////////////////
///////////// trace unit tests
//////////////////////////////////////////////////////////////////////

TEST(ProjectTests, test_trace_open_bad)
{
  int status = trace_open("test/no_trace_file.trace");
  ASSERT_EQ(status, -1) << "trace_open should return -1 if the file does not exist";
}

TEST(ProjectTests, test_trace_open_good)
{
  int status = trace_open("test/4_2_10.trace");
  ASSERT_NE(status, -1) << "trace_open should not return -1 if the file does exists";
}

TEST(ProjectTests, test_trace_atm_count)
{
  int status = trace_open("test/4_2_10.trace");
  ASSERT_NE(status, -1) << "trace_open should not return -1 if the file does exists";
  int atm_cnt = trace_atm_count();
  ASSERT_EQ(atm_cnt, 4) << "ATM count should be 4, received " << atm_cnt;
}

TEST(ProjectTests, test_trace_account_count)
{
  int status = trace_open("test/4_2_10.trace");
  ASSERT_NE(status, -1) << "trace_open should not return -1 if the file does exists";
  int account_cnt = trace_account_count();
  ASSERT_EQ(account_cnt, 2) << "account count should be 2, received " << account_cnt;
}

TEST(ProjectTests, test_trace_read_single_cmd)
{
  int status = trace_open("test/4_2_10.trace");
  ASSERT_NE(status, -1) << "trace_open should not return -1 if the file does exists";
  Command cmd;
  trace_read_cmd(&cmd);
  cmd_t c;
  int i, f, t, a;
  cmd_unpack(&cmd, &c, &i, &f, &t, &a);
  ASSERT_EQ(c, CONNECT) << "first command should be CONNECT, received " << c;
}

TEST(ProjectTests, test_trace_read_valid_commands)
{
  int status = trace_open("test/4_2_10.trace");
  ASSERT_NE(status, -1) << "trace_open should not return -1 if the file does exists";
  Command cmd;

  cmd_t c;
  int i, f, t, a;
  while (trace_read_cmd(&cmd) != 0)
  {
    cmd_unpack(&cmd, &c, &i, &f, &t, &a);
    switch (c)
    {
    case CONNECT:
    case EXIT:
    case DEPOSIT:
    case WITHDRAW:
    case TRANSFER:
    case BALANCE:
      break;
    default:
      ASSERT_TRUE(0) << "expected valid command, received " << c;
    }
  }

  trace_close();
}

TEST(ProjectTests, test_trace_read_connects)
{
  int status = trace_open("test/4_2_10.trace");
  ASSERT_NE(status, -1) << "trace_open should not return -1 if the file does exists";
  Command cmd;

  int connects = 0;

  cmd_t c;
  int i, f, t, a;
  while (trace_read_cmd(&cmd) != 0)
  {
    cmd_unpack(&cmd, &c, &i, &f, &t, &a);
    switch (c)
    {
    case CONNECT:
      connects++;
      break;
    }
  }

  ASSERT_EQ(connects, 4) << "there should be 4 connect messages, received " << connects;

  trace_close();
}

TEST(ProjectTests, test_trace_read_exits)
{
  int status = trace_open("test/4_2_10.trace");
  ASSERT_NE(status, -1) << "trace_open should not return -1 if the file does exists";
  Command cmd;

  int exits = 0;

  cmd_t c;
  int i, f, t, a;
  while (trace_read_cmd(&cmd) != 0)
  {
    cmd_unpack(&cmd, &c, &i, &f, &t, &a);
    switch (c)
    {
    case EXIT:
      exits++;
      break;
    }
  }

  ASSERT_EQ(exits, 4) << "there should be 4 exit messages, received " << exits;

  trace_close();
}

TEST(ProjectTests, test_trace_read_deposits)
{
  int status = trace_open("test/4_2_10.trace");
  ASSERT_NE(status, -1) << "trace_open should not return -1 if the file does exists";
  Command cmd;

  int deposits = 0;

  cmd_t c;
  int i, f, t, a;
  while (trace_read_cmd(&cmd) != 0)
  {
    cmd_unpack(&cmd, &c, &i, &f, &t, &a);
    switch (c)
    {
    case DEPOSIT:
      deposits++;
      break;
    }
  }

  ASSERT_EQ(deposits, 2) << "there should be 2 deposit messages, received " << deposits;

  trace_close();
}

TEST(ProjectTests, test_trace_read_withdraws)
{
  int status = trace_open("test/4_2_10.trace");
  ASSERT_NE(status, -1) << "trace_open should not return -1 if the file does exists";
  Command cmd;

  int withdraws = 0;

  cmd_t c;
  int i, f, t, a;
  while (trace_read_cmd(&cmd) != 0)
  {
    cmd_unpack(&cmd, &c, &i, &f, &t, &a);
    switch (c)
    {
    case WITHDRAW:
      withdraws++;
      break;
    }
  }

  ASSERT_EQ(withdraws, 3) << "there should be 3 withdraw messages, received " << withdraws;

  trace_close();
}

TEST(ProjectTests, test_trace_read_transfers)
{
  int status = trace_open("test/4_2_10.trace");
  ASSERT_NE(status, -1) << "trace_open should not return -1 if the file does exists";
  Command cmd;

  int transfers = 0;

  cmd_t c;
  int i, f, t, a;
  while (trace_read_cmd(&cmd) != 0)
  {
    cmd_unpack(&cmd, &c, &i, &f, &t, &a);
    switch (c)
    {
    case TRANSFER:
      transfers++;
      break;
    }
  }

  ASSERT_EQ(transfers, 4) << "there should be 4 transfer message, received " << transfers;

  trace_close();
}

TEST(ProjectTests, test_trace_read_balances)
{
  int status = trace_open("test/4_2_10.trace");
  ASSERT_NE(status, -1) << "trace_open should not return -1 if the file does exists";
  Command cmd;

  int balances = 0;

  cmd_t c;
  int i, f, t, a;
  while (trace_read_cmd(&cmd) != 0)
  {
    cmd_unpack(&cmd, &c, &i, &f, &t, &a);
    switch (c)
    {
    case BALANCE:
      balances++;
      break;
    }
  }

  ASSERT_EQ(balances, 3) << "there should be 3 balance messages, received " << balances;

  trace_close();
}

//////////////////////////////////////////////////////////////////////
///////////// atm unit tests
//////////////////////////////////////////////////////////////////////

TEST(ProjectTests, test_atm_invalid_command_status)
{
  // The file descriptors used for writing to the bank process and
  // reading in from the atm process.
  int fd[2];
  pipe(fd);
  int atm_read = fd[0];
  int bank_write = fd[1];

  // The atm message buffer.
  Command atmcmd;

  // Create an invalid command (99).
  cmd_pack(&atmcmd, 99, 0, 1, 2, 3);

  // Send it to the atm.
  int status = atm(bank_write, atm_read, 0, &atmcmd);

  ASSERT_EQ(status, ERR_UNKNOWN_CMD) << "atm should return ERR_UNKNOWN_CMD, received " << status;

  close(atm_read);
  close(bank_write);
}

TEST(ProjectTests, test_atm_unknown_atm)
{
  // The file descriptors used for writing to the bank process and
  // reading in from the atm process.
  int fd[2];
  pipe(fd);
  int atm_read = fd[0];
  int bank_write = fd[1];

  // The atm message buffer.
  Command atmcmd;

  // Create a connect message for ATM 0.
  MSG_CONNECT(&atmcmd, 0);

  // Send it to the atm with the incorrect ATM id.
  int status = atm(bank_write, atm_read, 1, &atmcmd);

  ASSERT_EQ(status, ERR_UNKNOWN_ATM) << "atm should return ERR_UNKNOWN_ATM, received " << status;

  close(atm_read);
  close(bank_write);
}

TEST(ProjectTests, test_atm_invalid_pipe_write)
{
  // Invalid pipe descriptors
  int atm_read = -1;
  int bank_write = -1;

  // The atm message buffer.
  Command atmcmd;

  MSG_CONNECT(&atmcmd, 0);

  // Send it to the atm.
  int status = atm(bank_write, atm_read, 0, &atmcmd);

  ASSERT_EQ(status, ERR_PIPE_WRITE_ERR) << "atm should return ERR_PIPE_WRITE_ERR, received " << status;
}

TEST(ProjectTests, test_atm_invalid_pipe_read)
{
  // The file descriptors used for writing to the bank process and
  // reading in from the atm process.
  int fd[2];
  pipe(fd);
  int bank_write = fd[1];
  // But, we invalidate the read pipe.
  int atm_read = -1;

  // The atm message buffer.
  Command atmcmd;

  MSG_CONNECT(&atmcmd, 0);

  // Send it to the atm.
  int status = atm(bank_write, atm_read, 0, &atmcmd);

  ASSERT_EQ(status, ERR_PIPE_READ_ERR) << "atm should return ERR_PIPE_READ_ERR, received " << status;
}

TEST(ProjectTests, test_atm_invalid_pipe_write_closed)
{
  // The file descriptors used for writing to the bank process and
  // reading in from the atm process.
  int fd[2];
  pipe(fd);
  int bank_write = fd[1];
  int atm_read = fd[0];

  // But, we close the write end of the pipe.
  close(bank_write);

  // Closing the write end of the pipe will cause a write to that pipe
  // to return 0 indicating EOF. The atm function must handle this
  // case. It will return a status indicating ERR_PIPE_WRITE_ERR.

  // The atm message buffer.
  Command atmcmd;

  MSG_CONNECT(&atmcmd, 0);

  // Send it to the atm.
  int status = atm(bank_write, atm_read, 0, &atmcmd);

  ASSERT_EQ(status, ERR_PIPE_WRITE_ERR) << "atm should return ERR_PIPE_WRITE_ERR, received " << status;
}


TEST(ProjectTests, test_atm_connect_success)
{
  // The file descriptors used for writing to the bank process and
  // reading in from the atm process.
  int bankfd[2];
  pipe(bankfd);
  int atmfd[2];
  pipe(atmfd);

  int bank_read = bankfd[0];
  int bank_write = bankfd[1];
  int atm_read = atmfd[0];
  int atm_write = atmfd[1];

  // The command buffer.
  Command cmd;

  MSG_CONNECT(&cmd, 0);

  if (fork() == 0)
  {
    // Send it to the atm.
    int status = atm(bank_write, atm_read, 0, &cmd);
    ASSERT_EQ(status, SUCCESS) << "status should be SUCCESS, received " << status;
    exit(0);
  }
  else
  {
    int flags = fcntl(bank_read, F_GETFL, 0);
    fcntl(bank_read, F_SETFL, flags | O_NONBLOCK);

    sleep(1);
    int status = read(bank_read, &cmd, MESSAGE_SIZE);
    ASSERT_NE(status, -1) << "ATM should have sent data to the bank";

    cmd_t c;
    int i, f, t, a;
    cmd_unpack(&cmd, &c, &i, &f, &t, &a);
    ASSERT_EQ(c, CONNECT) << "received message should be connect, got " << c;
    MSG_OK(&cmd, i, f, t, a);
    write(atm_write, &cmd, MESSAGE_SIZE);
  }
  // Close file descriptors.
  close(bankfd[0]);
  close(bankfd[1]);
  close(atmfd[0]);
  close(atmfd[1]);
}

TEST(ProjectTests, test_atm_exit_success)
{
  // The file descriptors used for writing to the bank process and
  // reading in from the atm process.
  int bankfd[2];
  pipe(bankfd);
  int atmfd[2];
  pipe(atmfd);

  int bank_read = bankfd[0];
  int bank_write = bankfd[1];
  int atm_read = atmfd[0];
  int atm_write = atmfd[1];

  // The command buffer.
  Command cmd;

  MSG_EXIT(&cmd, 0);

  if (fork() == 0)
  {
    // Send it to the atm.
    int status = atm(bank_write, atm_read, 0, &cmd);
    ASSERT_EQ(status, SUCCESS) << "status should be SUCCESS, received " << status;
    exit(0);
  }
  else
  {
    // Need to set the read end of pipe to non-blocking otherwise
    // students without an implementation will not be able to run
    // tests.
    int flags = fcntl(bank_read, F_GETFL, 0);
    fcntl(bank_read, F_SETFL, flags | O_NONBLOCK);

    sleep(1);
    int status = read(bank_read, &cmd, MESSAGE_SIZE);
    ASSERT_NE(status, -1) << "ATM should have sent data to the bank";

    cmd_t c;
    int i, f, t, a;
    cmd_unpack(&cmd, &c, &i, &f, &t, &a);
    ASSERT_EQ(c, EXIT) << "received message should be exit, got " << c;
    MSG_OK(&cmd, i, f, t, a);
    write(atm_write, &cmd, MESSAGE_SIZE);
  }
  // Close file descriptors.
  close(bankfd[0]);
  close(bankfd[1]);
  close(atmfd[0]);
  close(atmfd[1]);
}


//////////////////////////////////////////////////////////////////////
///////////// bank unit tests
//////////////////////////////////////////////////////////////////////

TEST(ProjectTests, test_bank_connect)
{
  int atmfd[2];
  pipe(atmfd);

  int atm_read = atmfd[0];
  int atm_write[1] = {atmfd[1]};

  // The command buffer.
  Command cmd;

  if (fork() == 0)
  {
    bank_open(1, 1);
    MSG_CONNECT(&cmd, 0);
    int atm_cnt = 1;
    int status = bank(atm_write, &cmd, &atm_cnt);
    ASSERT_EQ(status, SUCCESS) << "status should be SUCCESS, received " << status;
    bank_close();
    exit(0);
  }
  else
  {
    // Need to set the read end of pipe to non-blocking otherwise
    // students without an implementation will not be able to run
    // tests.
    int flags = fcntl(atm_read, F_GETFL, 0);
    fcntl(atm_read, F_SETFL, flags | O_NONBLOCK);

    sleep(1);
    int status = read(atm_read, &cmd, MESSAGE_SIZE);
    ASSERT_NE(status, -1) << "BANK should have received data from the ATM";

    cmd_t c;
    int i, f, t, a;
    cmd_unpack(&cmd, &c, &i, &f, &t, &a);
    ASSERT_EQ(c, OK) << "received message should be ok, got " << c;
    ASSERT_EQ(i, 0);
    ASSERT_EQ(f, -1);
    ASSERT_EQ(t, -1);
    ASSERT_EQ(a, -1);
  }
}

TEST(ProjectTests, test_bank_exit)
{
  int atmfd[2];
  pipe(atmfd);

  int atm_read = atmfd[0];
  int atm_write[1] = {atmfd[1]};

  // The command buffer.
  Command cmd;

  if (fork() == 0)
  {
    bank_open(1, 1);
    MSG_EXIT(&cmd, 0);
    int atm_cnt = 1;
    int status = bank(atm_write, &cmd, &atm_cnt);
    ASSERT_EQ(status, SUCCESS) << "status should be SUCCESS, received " << status;
    ASSERT_EQ(atm_cnt, 0) << "the ATM count should now be 0, received " << atm_cnt;
    bank_close();
    exit(0);
  }
  else
  {
    // Need to set the read end of pipe to non-blocking otherwise
    // students without an implementation will not be able to run
    // tests.
    int flags = fcntl(atm_read, F_GETFL, 0);
    fcntl(atm_read, F_SETFL, flags | O_NONBLOCK);

    sleep(1);
    int status = read(atm_read, &cmd, MESSAGE_SIZE);
    ASSERT_NE(status, -1) << "BANK should have received data from the ATM";

    cmd_t c;
    int i, f, t, a;
    cmd_unpack(&cmd, &c, &i, &f, &t, &a);
    ASSERT_EQ(c, OK) << "received message should be ok, got " << c;
    ASSERT_EQ(i, 0);
    ASSERT_EQ(f, -1);
    ASSERT_EQ(t, -1);
    ASSERT_EQ(a, -1);
  }
}

TEST(ProjectTests, test_bank_deposit)
{
  int atmfd[2];
  pipe(atmfd);

  int atm_read = atmfd[0];
  int atm_write[1] = {atmfd[1]};

  // The command buffer.
  Command cmd;

  if (fork() == 0)
  {
    bank_open(1, 1);
    MSG_DEPOSIT(&cmd, 0, 0, 200);
    int atm_cnt = 1;
    int status = bank(atm_write, &cmd, &atm_cnt);
    ASSERT_EQ(status, SUCCESS) << "status should be SUCCESS, received " << status;
    int *accounts = get_accounts();
    ASSERT_EQ(accounts[0], 200) << "account should have 200, found " << accounts[0];
    bank_close();
    exit(0);
  }
  else
  {
    // Need to set the read end of pipe to non-blocking otherwise
    // students without an implementation will not be able to run
    // tests.
    int flags = fcntl(atm_read, F_GETFL, 0);
    fcntl(atm_read, F_SETFL, flags | O_NONBLOCK);

    sleep(1);
    int status = read(atm_read, &cmd, MESSAGE_SIZE);
    ASSERT_NE(status, -1) << "BANK should have received data from the ATM";

    cmd_t c;
    int i, f, t, a;
    cmd_unpack(&cmd, &c, &i, &f, &t, &a);
    ASSERT_EQ(c, OK) << "received message should be ok, got " << c;
    ASSERT_EQ(i, 0);
    ASSERT_EQ(f, -1);
    ASSERT_EQ(t, 0);
    ASSERT_EQ(a, 200);
  }
}


