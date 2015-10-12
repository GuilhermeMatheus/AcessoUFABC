using System;
using System.Collections.Generic;
using System.Collections.Concurrent;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Newtonsoft.Json;
using System.IO;

namespace UFABC.AccessController.Server.Repository
{
    public class AccessRepository
    {
        private readonly string _jsonFileFullPath;
        private readonly ConcurrentDictionary<string, Response> _internalList;

        public readonly Response DEFAULT = new Response("Acesso negado.");

        public AccessRepository(string jsonFileFullPath)
        {
            _internalList = new ConcurrentDictionary<string, Response>();
            _jsonFileFullPath = jsonFileFullPath;
            LoadFile();
        }

        private void LoadFile()
        {
            if (!File.Exists(_jsonFileFullPath)) {
                SaveInternalList();
                return;
            }

            var json = String.Join("\n", File.ReadAllLines(_jsonFileFullPath));
            var items = JsonConvert.DeserializeObject<Dictionary<string, Response>>(json);

            foreach (var item in items)
                SetResponse(item.Key, item.Value);
        }

        private void SaveInternalList()
        {
            using (var file = File.CreateText(_jsonFileFullPath)) {
                var serializer = new JsonSerializer();
                serializer.Serialize(file, _internalList);
            }
        }

        public void SetResponse(string id, Response value)
        {
            _internalList.AddOrUpdate(id, value, (_, __) => value);
            SaveInternalList();
        }

        public void Delete(string id)
        {
            Response item;
            if (_internalList.TryRemove(id, out item))
                SaveInternalList();
        }

        public Dictionary<string, Response> GetAll()
        {
            return _internalList.ToDictionary(i => i.Key, i => i.Value);
        }

        public Response this[string index]
        {
            get
            {
                if (_internalList.ContainsKey(index))
                    return _internalList[index];

                return new Response(message: index + " Acesso negado");
            }
        }

        public bool TryGet(string id, out Response value)
        {
            var contains = _internalList.ContainsKey(id);
            value = contains ? _internalList[id] : default(Response);

            return contains;
        }
    }
}
