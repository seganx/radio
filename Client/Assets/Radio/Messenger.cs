﻿using UnityEngine;
using System.Net;

namespace SeganX.Realtime.Internal
{
    public class Messenger
    {
        private const string logName = "[Network] [Messenger]";

        private ClientInfo clientInfo = new ClientInfo();
        private readonly Transmitter transmitter = new Transmitter();
        private readonly BufferWriter sendBuffer = new BufferWriter(256);

        public bool Started => clientInfo.device != null;
        public bool Stopped => clientInfo.device == null;
        public uint Token => clientInfo.token;
        public short Id => clientInfo.id;
        public short Room => clientInfo.room;
        public sbyte Index => clientInfo.index;
        public bool Loggedin => clientInfo.token != 0;

        public float DelayFactor { get; set; } = 1;
        public Flag Flag { get; private set; } = 0;
        public ulong ServerTime { get; private set; } = 0;

        public void Start(byte[] devicebytes, IPEndPoint serverAddress, System.Action<Error, sbyte, BufferReader, byte> OnReceivedMessage)
        {
            if (Started)
            {
                Debug.LogWarning("[Network] Already started!");
                return;
            }

            if (devicebytes.Length != 32)
            {
                Debug.LogError("[Network] Start: Device id must be 32 bytes");
                return;
            }

            clientInfo.device = devicebytes;
            transmitter.Start(clientInfo, serverAddress, OnReceivedMessage);
        }

        public void Stop()
        {
            if (Stopped) return;
            Logout(null);
            transmitter.Stop();
            clientInfo = new ClientInfo();
        }

        public void Update(float elapsedTime)
        {
            transmitter.Update(elapsedTime);
        }

        public void Login(System.Action<Error> callback)
        {
            if (Loggedin)
            {
                callback(Error.NoError);
                return;
            }

            sendBuffer.Reset()
                .AppendByte((byte)MessageType.Login)
                .AppendBytes(clientInfo.device, 32)
                .AppendUint(ComputeChecksum(sendBuffer.Bytes, sendBuffer.Length));

            Debug.Log($"{logName} Login {clientInfo.device}");
            transmitter.SendRequestToServer(MessageType.Login, sendBuffer.Bytes, sendBuffer.Length, DelayFactor, (error, buffer) =>
            {
                if (error == Error.NoError)
                {
                    var rectoken = buffer.ReadUint();
                    var reclobby = buffer.ReadShort();
                    var recroom = buffer.ReadShort();
                    var recindex = buffer.ReadSbyte();
                    var checksum = buffer.ReadUint();
                    if (checksum == ComputeChecksum(buffer.Bytes, 11))
                    {
                        clientInfo.token = rectoken;
                        clientInfo.id = reclobby;
                        clientInfo.room = recroom;
                        clientInfo.index = recindex;

                        Debug.Log($"{logName} Login response Token:{clientInfo.token} Id:{clientInfo.id} Room:{clientInfo.room} Index:{clientInfo.index}");
                        callback?.Invoke(error);
                    }
                    else callback?.Invoke(Error.Invalid);
                }
                else callback?.Invoke(error);

            });
        }

        public void Logout(System.Action callback)
        {
            if (clientInfo.token == 0 || clientInfo.device == null) return;

            sendBuffer.Reset()
                .AppendByte((byte)MessageType.Logout)
                .AppendUint(clientInfo.token)
                .AppendShort(clientInfo.id)
                .AppendShort(clientInfo.room)
                .AppendSbyte(clientInfo.index)
                .AppendUint(ComputeChecksum(sendBuffer.Bytes, sendBuffer.Length));

            transmitter.SendRequestToServer(MessageType.Logout, sendBuffer.Bytes, sendBuffer.Length, DelayFactor, (error, buffer) => callback?.Invoke());
        }

        public void SendPing(System.Action<Error, ulong> callback)
        {
            if (clientInfo.token == 0 || clientInfo.device == null) return;

            ulong Tick() => (ulong)System.DateTimeOffset.Now.ToUnixTimeMilliseconds();

            sendBuffer.Reset()
                .AppendByte((byte)MessageType.Ping)
                .AppendUint(clientInfo.token)
                .AppendShort(clientInfo.id)
                .AppendShort(clientInfo.room)
                .AppendSbyte(clientInfo.index)
                .AppendUlong(Tick());

            transmitter.SendRequestToServer(MessageType.Ping, sendBuffer.Bytes, sendBuffer.Length, DelayFactor, (error, buffer) =>
            {
                ulong sentTick = buffer.ReadUlong();
                ServerTime = buffer.ReadUlong();
                Flag = (Flag)buffer.ReadByte();
                callback?.Invoke(error, Tick() - sentTick);
            });
        }

        public void CreateRoom(ushort openTimeout, byte[] properties, MatchmakingParams matchmaking, System.Action<Error, short, sbyte> callback)
        {
            if (clientInfo.device == null || clientInfo.token == 0) return;

#if UNITY_EDITOR
            if (properties.Length != 32)
            {
                Debug.LogError($"{logName} CreateRoom: Length of property must be 32 bytes!");
                return;
            }
#endif

            sendBuffer.Reset()
                .AppendByte((byte)MessageType.CreateRoom)
                .AppendUint(clientInfo.token)
                .AppendShort(clientInfo.id)
                .AppendUshort(openTimeout)
                .AppendBytes(properties, 32)
                .AppendInt(matchmaking.a)
                .AppendInt(matchmaking.b)
                .AppendInt(matchmaking.c)
                .AppendInt(matchmaking.d);

            Debug.Log($"{logName} Create room Token:{clientInfo.token} Id:{clientInfo.id} Timeout:{openTimeout} Matchmaking:{matchmaking}");
            transmitter.SendRequestToServer(MessageType.CreateRoom, sendBuffer.Bytes, sendBuffer.Length, DelayFactor, (error, buffer) =>
            {

                if (error == Error.NoError)
                {
                    clientInfo.room = buffer.ReadShort();
                    clientInfo.index = buffer.ReadSbyte();
                    Flag = (Flag)buffer.ReadByte();
                    Debug.Log($"{logName} Create Room response Token:{clientInfo.token} Id:{clientInfo.id} Room:{clientInfo.room} Index:{clientInfo.index}");
                    callback?.Invoke(error, clientInfo.room, clientInfo.index);
                }
                else callback?.Invoke(error, -1, -1);
            });
        }

        public void JoinRoom(MatchmakingRanges matchmaking, System.Action<Error, short, sbyte, byte[]> callback = null)
        {
            if (clientInfo.device == null || clientInfo.token == 0) return;

            sendBuffer.Reset()
                .AppendByte((byte)MessageType.Join)
                .AppendUint(clientInfo.token)
                .AppendShort(clientInfo.id)
                .AppendInt(matchmaking.aMin)
                .AppendInt(matchmaking.aMax)
                .AppendInt(matchmaking.bMin)
                .AppendInt(matchmaking.bMax)
                .AppendInt(matchmaking.cMin)
                .AppendInt(matchmaking.cMax)
                .AppendInt(matchmaking.dMin)
                .AppendInt(matchmaking.dMax);

            Debug.Log($"{logName} Join room Token:{clientInfo.token} Id:{clientInfo.id} Matchmaking:{matchmaking}");
            transmitter.SendRequestToServer(MessageType.Join, sendBuffer.Bytes, sendBuffer.Length, DelayFactor, (error, buffer) =>
            {
                if (error == Error.NoError)
                {
                    clientInfo.room = buffer.ReadShort();
                    clientInfo.index = buffer.ReadSbyte();
                    Flag = (Flag)buffer.ReadByte();

                    var properties = new byte[32];
                    buffer.ReadBytes(properties, 32);

                    Debug.Log($"{logName} Join Room response Token:{clientInfo.token} Id:{clientInfo.id} Room:{clientInfo.room} Index:{clientInfo.index}");
                    callback?.Invoke(error, clientInfo.room, clientInfo.index, properties);
                }
                else callback?.Invoke(error, -1, -1, null);

            });
        }

        public void LeaveRoom(System.Action<Error> callback)
        {
            if (clientInfo.device == null || clientInfo.token == 0) return;

            sendBuffer.Reset()
                .AppendByte((byte)MessageType.Leave)
                .AppendUint(clientInfo.token)
                .AppendShort(clientInfo.id)
                .AppendShort(clientInfo.room)
                .AppendSbyte(clientInfo.index);

            Debug.Log($"{logName} Leave Token:{clientInfo.token} Id:{clientInfo.id} Room:{clientInfo.room} Index:{clientInfo.index}");
            transmitter.SendRequestToServer(MessageType.Leave, sendBuffer.Bytes, sendBuffer.Length, DelayFactor, (error, buffer) =>
            {
                clientInfo.room = -1;
                clientInfo.index = -1;
                Debug.Log($"{logName} Leave response Token:{clientInfo.token} Id:{clientInfo.id} Room:{clientInfo.room} Index:{clientInfo.index}");
                callback?.Invoke(error);
            });
        }

        public void SendUnreliable(Target target, BufferWriter data, sbyte targetId = -1)
        {
            if (clientInfo.device == null || clientInfo.token == 0) return;
            transmitter.SendMessageUnreliable(target, data.Bytes, (byte)data.Length, targetId);
        }

        public void SendReliable(sbyte targetId, BufferWriter data)
        {
            if (clientInfo.device == null || clientInfo.token == 0) return;
            transmitter.SendMessageReliable(targetId, data.Bytes, (byte)data.Length);
        }

        private uint ComputeChecksum(byte[] buffer, int length)
        {
            uint checksum = 0;
            for (int i = 0; i < length; i++)
                checksum += 64548 + (uint)buffer[i] * 6597;
            return checksum;
        }
    }
}